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

#if HAVE_WINDOWS_H
void fs_absolute(const char* path, char* buf, int size)
{
	GetFullPathName(path, size, buf, NULL);
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
void fs_absolute(const char* path, char* buf, int size)
{
	realpath(path, buf);
}

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
char* fs_current_path(char* buf, int size)
{
	if (GetCurrentDirectory(size, buf))
	{
		return buf;
	}

	return NULL;
}
#else
#ifdef HAVE_UNISTD_H
char* fs_current_path(char* buf, int size)
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
	stat(path, &s);
	return s.st_mode & S_IFDIR;
}

int fs_is_file(const char* path)
{
	struct stat s;
	stat(path, &s);
	return s.st_mode & S_IFREG;
}

int fs_is_symlink(const char* path)
{
#ifndef HAVE_WINDOWS_H
	struct stat s;
	stat(path, &s);
	return s.st_mode & S_IFLNK;
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
		return -1;
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);

	fclose(file);
	return size;
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

#ifdef HAVE_WINDOWS_H
void fs_temp_directory_path(char* buf, int size)
{
	GetTempPath(size, buf);
}
#else
#ifdef HAVE_STDLIB_H
void fs_temp_directory_path(char* buf, int size)
{
	char* path = getenv("TMPDIR");
}
#endif
#endif