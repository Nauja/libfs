#include "fs.h"

#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#ifdef HAVE_SYS_STAT_H
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
#include <unistd.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#include <strsafe.h>
#endif

#define FS_TRUE 1

typedef struct fs_hooks fs_hooks;
typedef struct fs_directory_iterator fs_directory_iterator;

#if defined(_MSC_VER)
/* work around MSVC error C2322: '...' address of dllimport '...' is not static */
static void* LIBFS_CDECL internal_malloc(size_t size)
{
	return LIBFS_MALLOC(size);
}

static void LIBFS_CDECL internal_free(void* pointer)
{
	LIBFS_FREE(pointer);
}
#else
#define internal_malloc LIBFS_MALLOC
#define internal_free LIBFS_FREE
#endif

static fs_hooks fs_global_hooks = {
	internal_malloc,
	internal_free
};

void fs_init_hooks(struct fs_hooks* hooks)
{
	fs_global_hooks.malloc_fn = hooks->malloc_fn;
	fs_global_hooks.free_fn = hooks->free_fn;
}

#if HAVE_WINDOWS_H
char* fs_absolute(const char* path, char* buf, int size)
{
	if (!GetFullPathName(path, size, buf, NULL))
	{
		return NULL;
	}

	return buf;
}

void fs_copy(const char* from, const char* to)
{
	fs_copy_file(from, to);
}

void fs_copy_file(const char* from, const char* to)
{
	CopyFile(from, to, 0);
}
#else
#if HAVE_STDLIB_H
char* fs_absolute(const char* path, char* buf, int size)
{
	if (!realpath(path, buf))
	{
		return NULL;
	}

	return buf;
}
#endif

#if HAVE_SENDFILE_H
void fs_copy(const char* from, const char* to)
{
	fs_copy_file(from, to);
}

void fs_copy_file(const char* from, const char* to)
{
	FILE* ffrom = fopen(from, "rb");
	if (!ffrom)
	{
		return;
	}

	FILE* fto = fopen(to, "wb");
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
char* fs_current_dir(char* buf, int size)
{
	if (GetCurrentDirectory(size, buf))
	{
		return buf;
	}

	return NULL;
}
#else
#ifdef HAVE_UNISTD_H
char* fs_current_dir(char* buf, int size)
{
	return getcwd(buf, size);
}
#endif
#endif

#if HAVE_SYS_STAT_H
int fs_exists(const char* path)
{
	struct stat s;
	return stat(path, &s) == 0;
}

int fs_is_directory(const char* path)
{
	struct stat s;
	return (stat(path, &s) == 0) && (s.st_mode & S_IFDIR);
}

int fs_is_file(const char* path)
{
	struct stat s;
	return (stat(path, &s) == 0) && (s.st_mode & S_IFREG);
}

int fs_is_symlink(const char* path)
{
#ifndef HAVE_WINDOWS_H
	struct stat s;
	return (stat(path, &s) == 0) && (s.st_mode & S_IFLNK);
#else
	return 0;
#endif
}
#endif

#ifdef HAVE_STDIO_H
long fs_file_size(const char* path)
{
	FILE* file = fopen(path, "rb");
	if (!file)
	{
		return -1L;
	}

	struct stat stat;
	if (fstat(fileno(file), &stat) == -1)
	{
		return -1;
	}

	fclose(file);
	return stat.st_size;
}

void* fs_read_file(const char* path, int* size)
{
	FILE* file = fopen(path, "rb");
	if (!file)
	{
		return NULL;
	}

	// Check file size
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Read file content
	void* data = fs_global_hooks.malloc_fn(length);
	if (data) fread(data, length, 1, file);
	fclose(file);

	*size = length;
	return data;
}
#endif

#ifdef HAVE_STRING_H
int fs_join_path(char* buf, int size, const char* left, const char* right)
{
	return snprintf(buf, size, "%s/%s", left, right);
}
#endif

#if defined(HAVE_WINDOWS_H)
char* fs_temp_dir(char* buf, int size)
{
	if (!GetTempPath(size, buf))
	{
		return NULL;
	}

	return buf;
}
#elif defined(HAVE_STDLIB_H)
char* fs_temp_dir(char* buf, int size)
{
	const char* path = getenv("TMPDIR");
	if (!path)
	{
		return NULL;
	}

	snprintf(buf, size, "%s", path);
	return buf;
}
#endif

#if defined(HAVE_WINDOWS_H)
typedef struct fs_win_directory_iterator
{
	struct fs_directory_iterator base;
	WIN32_FIND_DATA fdFile;
	HANDLE hFind;
	size_t started;
} fs_win_directory_iterator;

struct fs_directory_iterator* fs_open_dir(const char* path)
{
	fs_win_directory_iterator* it = (fs_win_directory_iterator*)fs_global_hooks.malloc_fn(sizeof(fs_win_directory_iterator));
	memset(it, 0, sizeof(fs_win_directory_iterator));

	TCHAR szDir[MAX_PATH];
	StringCchCopy(szDir, MAX_PATH, path);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	if ((it->hFind = FindFirstFile(szDir, &it->fdFile)) == INVALID_HANDLE_VALUE)
	{
		free(it);
		return NULL;
	}

	return (fs_directory_iterator*)it;
}

struct fs_directory_iterator* fs_read_dir(struct fs_directory_iterator* it)
{
	fs_win_directory_iterator* _it = (fs_win_directory_iterator*)it;

	if (!_it->started)
	{
		_it->started = FS_TRUE;
	}
	else if (!FindNextFile(_it->hFind, &_it->fdFile))
	{
		return NULL;
	}

	_it->base.path = _it->fdFile.cFileName;
	return it;
}

void fs_close_dir(struct fs_directory_iterator* it)
{
	fs_win_directory_iterator* _it = (fs_win_directory_iterator*)it;
	FindClose(_it->hFind);
	free(_it);
}
#elif defined(HAVE_DIRENT_H)
typedef struct fs_posix_directory_iterator
{
	struct fs_directory_iterator base;
	DIR* dir;
	struct dirent* ent;
} fs_posix_directory_iterator;

struct fs_directory_iterator* fs_open_dir(const char* path)
{
	fs_posix_directory_iterator* it = (fs_posix_directory_iterator*)fs_global_hooks.malloc_fn(sizeof(fs_posix_directory_iterator));
	memset(it, 0, sizeof(fs_posix_directory_iterator));

	if (!(it->dir = opendir(path)))
	{
		free(it);
		return NULL;
	}

	return it;
}

struct fs_directory_iterator* fs_read_dir(struct fs_directory_iterator* it)
{
	fs_posix_directory_iterator* _it = (fs_posix_directory_iterator*)it;
	if (!(_it->ent = readdir(_it->dir)))
	{
		return NULL;
	}

	_it->base.path = &_it->ent->d_name[0];
	return _it;
}

void fs_close_dir(struct fs_directory_iterator* it)
{
	fs_posix_directory_iterator* _it = (fs_posix_directory_iterator*)it;
	closedir(_it->dir);
	free(_it);
}
#endif