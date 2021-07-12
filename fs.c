#include "fs.h"

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
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
#endif

#ifdef HAVE_WINDOWS_H
char* fs_get_cwd(char* buf, int size)
{
	if (GetCurrentDirectory(size, buf))
	{
		return buf;
	}

	return NULL;
}
#else
#ifdef HAVE_UNISTD_H
char* fs_get_cwd(char* buf, int size)
{
	return getcwd(buf, size);
}
#endif
#endif

#if HAVE_SYS_STAT_H
int fs_exists(const char* path)
{
	struct stat s;
	return stat(path, &s);
}

int fs_is_directory(const char* path)
{
	struct stat s;
	stat(path, &s);
	return s.st_mode & S_IFDIR;
}

int fs_is_file(const char* path)
{
	struct stat s;
	stat(path, &s);
	return s.st_mode & S_IFREG;
}
#endif

#ifdef HAVE_STDIO_H
void* fs_read_file(const char* path, int* size)
{
	FILE* file = NULL;
	errno_t err = fopen_s(&file, path, "rb");
	if (!file)
	{
		return NULL;
	}

	// Check file size
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Read file content
	void* data = LIBFS_MALLOC(length);
	if (data) fread(data, length, 1, file);
	fclose(file);

	return data;
}
#endif

#ifdef HAVE_STRING_H
int fs_join_path(char* buf, int size, const char* left, const char* right)
{
	return snprintf(buf, size, "%s/%s", left, right);
}
#endif
