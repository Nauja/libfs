#include "fs.h"

#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <errno.h>
#include <sys/stat.h>
#endif
#ifdef HAVE_SYS_SENDFILE_H
#include <sys/sendfile.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_UNISTD_H
#include <errno.h>
#include <unistd.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#include <strsafe.h>
#endif

#ifndef realpath
char *realpath(const char *path, char *resolved_path);
#endif

#ifndef snprintf
int snprintf(char *s, size_t n, const char *format, ...);
#endif

#ifndef fileno
int fileno(FILE *stream);
#endif

#define LIBFS_FALSE 0
#define LIBFS_TRUE 1
#define LIBFS_MKDIR_PERMISSIONS 0700
#define LIBFS_UNUSED(x) (void)(x)

typedef struct fs_hooks fs_hooks;
typedef struct fs_directory_iterator fs_directory_iterator;

#if defined(_MSC_VER)
/* work around MSVC error C2322: '...' address of dllimport '...' is not static */
static void *LIBFS_CDECL internal_malloc(size_t size)
{
	return LIBFS_MALLOC(size);
}

static void LIBFS_CDECL internal_free(void *pointer)
{
	LIBFS_FREE(pointer);
}
#else
#define internal_malloc LIBFS_MALLOC
#define internal_free LIBFS_FREE
#endif

static fs_hooks fs_global_hooks = {
	internal_malloc,
	internal_free};

LIBFS_PUBLIC(void)
fs_init_hooks(fs_hooks *hooks)
{
	fs_global_hooks.malloc_fn = hooks->malloc_fn;
	fs_global_hooks.free_fn = hooks->free_fn;
}

#define _LIBFS_MALLOC fs_global_hooks.malloc_fn
#define _LIBFS_FREE fs_global_hooks.free_fn

#if HAVE_WINDOWS_H
LIBFS_PUBLIC(char *)
fs_absolute(const char *path, char *buf, size_t size)
{
	LIBFS_UNUSED(size);
	if (!GetFullPathName(path, (DWORD)size, buf, NULL))
	{
		return NULL;
	}

	return buf;
}

LIBFS_PUBLIC(void)
fs_copy(const char *from, const char *to)
{
	fs_copy_file(from, to);
}

LIBFS_PUBLIC(void)
fs_copy_file(const char *from, const char *to)
{
	CopyFile(from, to, 0);
}
#else
#if HAVE_STDLIB_H
LIBFS_PUBLIC(char *)
fs_absolute(const char *path, char *buf, size_t size)
{
	LIBFS_UNUSED(size);
	if (!realpath(path, buf))
	{
		return NULL;
	}

	return buf;
}
#endif

#if HAVE_SENDFILE_H
LIBFS_PUBLIC(void)
fs_copy(const char *from, const char *to)
{
	fs_copy_file(from, to);
}

LIBFS_PUBLIC(void)
fs_copy_file(const char *from, const char *to)
{
	FILE *ffrom = fopen(from, "rb");
	if (!ffrom)
	{
		return;
	}

	FILE *fto = fopen(to, "wb");
	if (!fto)
	{
		return;
	}

	fseek(ffrom, 0, SEEK_END);
	long size = ftell(ffrom);
	fseek(ffrom, 0, 0);

	sendfile(ffrom, fto, 0, size);

	fclose(ffrom);
	fclose(fto);
}
#endif
#endif

#ifdef HAVE_WINDOWS_H
LIBFS_PUBLIC(char *)
fs_current_dir(char *buf, size_t size)
{
	if (GetCurrentDirectory((DWORD)size, buf))
	{
		return buf;
	}

	return NULL;
}
#else
#ifdef HAVE_UNISTD_H
LIBFS_PUBLIC(char *)
fs_current_dir(char *buf, size_t size)
{
	return getcwd(buf, size);
}
#endif
#endif

#if HAVE_SYS_STAT_H
LIBFS_PUBLIC(int)
fs_exist(const char *path)
{
	struct stat s;
	return stat(path, &s) == 0;
}

LIBFS_PUBLIC(int)
fs_is_directory(const char *path)
{
	struct stat s;
	return (stat(path, &s) == 0) && S_ISDIR(s.st_mode);
}

LIBFS_PUBLIC(int)
fs_is_file(const char *path)
{
	struct stat s;
	return (stat(path, &s) == 0) && S_ISREG(s.st_mode);
}

LIBFS_PUBLIC(int)
fs_is_symlink(const char *path)
{
#ifndef HAVE_WINDOWS_H
	struct stat s;
	LIBFS_UNUSED(path);
	return (stat(path, &s) == 0) && S_ISLNK(s.st_mode);
#else
	LIBFS_UNUSED(path);
	return 0;
#endif
}
#endif

#ifdef HAVE_STDIO_H
LIBFS_PUBLIC(size_t)
fs_file_size(const char *path)
{
	struct stat stat;
	FILE *file = fopen(path, "rb");
	if (!file)
	{
		return -1L;
	}

	if (fstat(fileno(file), &stat) == -1)
	{
		return -1;
	}

	fclose(file);
	return stat.st_size;
}

static void *
fs_read_file_internal(const char *path, void *buf, size_t size, size_t *readen)
{
	void *data;
	size_t file_size;
	size_t read_size;
	FILE *file = fopen(path, "rb");
	if (!file)
	{
		return NULL;
	}

	/* File size */
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	data = buf;
	read_size = size;
	if (!data)
	{
		read_size = file_size;
		size = file_size + 1;

		/* Create a buffer large enough */
		data = _LIBFS_MALLOC(size);
		if (!data)
		{
			fclose(file);
			return NULL;
		}
	}

	if (size > 0)
	{
		read_size = fread(data, 1, read_size, file);

		/* Append \0 */
		if (read_size < size)
		{
			((char *)data)[read_size] = '\0';
		}
		else
		{
			((char *)data)[size - 1] = '\0';
		}
	}

	fclose(file);

	*readen = file_size;
	return data;
}

LIBFS_PUBLIC(size_t)
fs_read_file_buffer(const char *path, void *buf, size_t size)
{
	size_t readen = 0;
	fs_read_file_internal(path, buf, size, &readen);
	return readen;
}

LIBFS_PUBLIC(void *)
fs_read_file(const char *path, size_t *size)
{
	return fs_read_file_internal(path, NULL, 0, size);
}

LIBFS_PUBLIC(int)
fs_write_file(const char *path, const void *buf, size_t size)
{
	FILE *file = fopen(path, "wb");
	if (!file)
	{
		return LIBFS_FALSE;
	}

	fwrite(buf, size, 1, file);
	fclose(file);
	return LIBFS_TRUE;
}

typedef struct fs_file_iterator
{
	FILE *file;
} fs_file_iterator;

LIBFS_PUBLIC(fs_file_iterator *)
fs_iter_file(const char *path)
{
	fs_file_iterator *it;
	FILE *f = fopen(path, "r");
	if (!f)
	{
		return NULL;
	}

	it = (fs_file_iterator *)_LIBFS_MALLOC(sizeof(fs_file_iterator));
	memset(it, 0, sizeof(fs_file_iterator));
	it->file = f;
	return it;
}

LIBFS_PUBLIC(fs_file_iterator *)
fs_next_char(fs_file_iterator *it, char *c)
{
	if (!fread(c, 1, 1, it->file))
	{
		return NULL;
	}

	return it;
}

LIBFS_PUBLIC(void)
fs_close_file(fs_file_iterator *it)
{
	fclose(it->file);
	_LIBFS_FREE(it);
}
#endif

#ifdef HAVE_STRING_H
LIBFS_PUBLIC(size_t)
fs_join_path(char *buf, size_t size, const char *left, const char *right)
{
	return snprintf(buf, size, "%s/%s", left, right);
}
#endif

#if defined(HAVE_WINDOWS_H)
LIBFS_PUBLIC(char *)
fs_temp_dir(char *buf, size_t size)
{
	if (!GetTempPath((DWORD)size, buf))
	{
		return NULL;
	}

	return buf;
}

LIBFS_PUBLIC(int)
fs_delete_dir(const char *path)
{
	if (RemoveDirectory(path) == 0)
	{
		return ERROR_FILE_NOT_FOUND == GetLastError();
	}

	return LIBFS_TRUE;
}

LIBFS_PUBLIC(int)
fs_delete_file(const char *path)
{
	if (DeleteFile(path) == 0)
	{
		return ERROR_FILE_NOT_FOUND == GetLastError();
	}

	return LIBFS_TRUE;
}

LIBFS_PUBLIC(int)
fs_make_dir(const char *path)
{
	if (CreateDirectory(path, NULL) == 0)
	{
		return ERROR_ALREADY_EXISTS == GetLastError();
	}

	return LIBFS_TRUE;
}
#else /* !HAVE_WINDOWS_H */
#if defined(HAVE_STDLIB_H)
LIBFS_PUBLIC(char *)
fs_temp_dir(char *buf, size_t size)
{
	const char *path = getenv("TMPDIR");
	if (!path)
	{
		return NULL;
	}

	snprintf(buf, size, "%s", path);
	return buf;
}
#endif

#ifdef HAVE_UNISTD_H
LIBFS_PUBLIC(int)
fs_delete_dir(const char *path)
{
	return (rmdir(path) == 0) || (ENOENT == errno);
}
#endif

#ifdef HAVE_STDIO_H
LIBFS_PUBLIC(int)
fs_delete_file(const char *path)
{
	return (remove(path) == 0) || (ENOENT == errno);
}
#endif

#ifdef HAVE_SYS_STAT_H
LIBFS_PUBLIC(int)
fs_make_dir(const char *path)
{
	return (mkdir(path, LIBFS_MKDIR_PERMISSIONS) == 0) || (EEXIST == errno);
}
#endif
#endif /* HAVE_WINDOWS_H */

#if defined(HAVE_WINDOWS_H)
typedef struct fs_win_directory_iterator
{
	fs_directory_iterator base;
	WIN32_FIND_DATA fdFile;
	HANDLE hFind;
	size_t started;
} fs_win_directory_iterator;

LIBFS_PUBLIC(fs_directory_iterator *)
fs_open_dir(const char *path)
{
	fs_win_directory_iterator *it;
	TCHAR szDir[MAX_PATH];
	WIN32_FIND_DATA fdFile;
	HANDLE hFind;
	StringCchCopy(szDir, MAX_PATH, path);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
	hFind = FindFirstFile(szDir, &fdFile);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	it = (fs_win_directory_iterator *)_LIBFS_MALLOC(sizeof(fs_win_directory_iterator));
	memset(it, 0, sizeof(fs_win_directory_iterator));
	it->fdFile = fdFile;
	it->hFind = hFind;
	return (fs_directory_iterator *)it;
}

LIBFS_PUBLIC(fs_directory_iterator *)
fs_read_dir(fs_directory_iterator *it)
{
	fs_win_directory_iterator *_it = (fs_win_directory_iterator *)it;

	if (!_it->started)
	{
		_it->started = LIBFS_TRUE;
	}
	else if (!FindNextFile(_it->hFind, &_it->fdFile))
	{
		return NULL;
	}

	_it->base.path = _it->fdFile.cFileName;
	return it;
}

LIBFS_PUBLIC(void)
fs_close_dir(fs_directory_iterator *it)
{
	fs_win_directory_iterator *_it = (fs_win_directory_iterator *)it;
	FindClose(_it->hFind);
	_LIBFS_FREE(_it);
}
#elif defined(HAVE_DIRENT_H)
typedef struct fs_posix_directory_iterator
{
	fs_directory_iterator base;
	DIR *dir;
	struct dirent *ent;
} fs_posix_directory_iterator;

LIBFS_PUBLIC(fs_directory_iterator *)
fs_open_dir(const char *path)
{
	fs_posix_directory_iterator *it;
	DIR *d = opendir(path);
	if (!d)
	{
		return NULL;
	}

	it = (fs_posix_directory_iterator *)_LIBFS_MALLOC(sizeof(fs_posix_directory_iterator));
	memset(it, 0, sizeof(fs_posix_directory_iterator));
	it->dir = d;
	return (fs_directory_iterator *)it;
}

LIBFS_PUBLIC(fs_directory_iterator *)
fs_read_dir(fs_directory_iterator *it)
{
	fs_posix_directory_iterator *_it = (fs_posix_directory_iterator *)it;
	if (!(_it->ent = readdir(_it->dir)))
	{
		return NULL;
	}

	_it->base.path = &_it->ent->d_name[0];
	return it;
}

LIBFS_PUBLIC(void)
fs_close_dir(fs_directory_iterator *it)
{
	fs_posix_directory_iterator *_it = (fs_posix_directory_iterator *)it;
	closedir(_it->dir);
	_LIBFS_FREE(_it);
}
#endif
