#ifndef LIBFS__h
#define LIBFS__h

#ifdef __cplusplus
extern "C"
{
#endif

/* project version */
#define LIBFS_VERSION_MAJOR 0
#define LIBFS_VERSION_MINOR 1
#define LIBFS_VERSION_PATCH 0

/* Define to 1 if you have the <dirent.h> header file. */
#ifndef HAVE_DIRENT_H
#cmakedefine HAVE_DIRENT_H 1
#endif

/* Define to 1 if you have the <stdio.h> header file. */
#ifndef HAVE_STDIO_H
#cmakedefine HAVE_STDIO_H 1
#endif

/* Define to 1 if you have the <stdlib.h> header file. */
#ifndef HAVE_STDLIB_H
#cmakedefine HAVE_STDLIB_H 1
#endif

/* Define to 1 if you have the <sys/stat.h> header file. */
#ifndef HAVE_SYS_STAT_H
#cmakedefine HAVE_SYS_STAT_H 1
#endif

/* Define to 1 if you have the <sys/sendfile.h> header file. */
#ifndef HAVE_SYS_SENDFILE_H
#cmakedefine HAVE_SYS_SENDFILE_H 1
#endif

/* Define to 1 if you have the <string.h> header file. */
#ifndef HAVE_STRING_H
#cmakedefine HAVE_STRING_H 1
#endif

/* Define to 1 if you have the <unistd.h> header file. */
#ifndef HAVE_UNISTD_H
#cmakedefine HAVE_UNISTD_H 1
#endif

/* Define to 1 if you have the <windows.h> header file. */
#ifndef HAVE_WINDOWS_H
#cmakedefine HAVE_WINDOWS_H 1
#endif

/* Define to 1 if you have the `free' function. */
#ifndef HAVE_FREE
#cmakedefine HAVE_FREE 1
#endif

/* Define to 1 if you have the `malloc' function. */
#ifndef HAVE_MALLOC
#cmakedefine HAVE_MALLOC 1
#endif

/* Define to 1 if you have the `memset' function. */
#ifndef HAVE_MEMSET
#cmakedefine HAVE_MEMSET 1
#endif

/* Define to 1 if you have the `memcpy' function. */
#ifndef HAVE_MEMCPY
#cmakedefine HAVE_MEMCPY 1
#endif

/* Define to 1 if you have the `_snprintf_s' function. */
#ifndef HAVE__SNPRINTF_S
#cmakedefine HAVE__SNPRINTF_S 1
#endif

/* Define to 1 if you have the `_snprintf' function. */
#ifndef HAVE__SNPRINTF
#cmakedefine HAVE__SNPRINTF 1
#endif

/* Define to 1 if you have the `snprintf' function. */
#ifndef HAVE_SNPRINTF
#cmakedefine HAVE_SNPRINTF 1
#endif

/* Define to 1 if you have the `vsnprintf' function. */
#ifndef HAVE_VSNPRINTF
#cmakedefine HAVE_VSNPRINTF 1
#endif

#ifndef LIBFS_MALLOC
#ifdef HAVE_MALLOC
#define LIBFS_MALLOC malloc
#else
#define LIBFS_MALLOC(size) NULL
#endif
#endif

#ifndef LIBFS_FREE
#ifdef HAVE_FREE
#define LIBFS_FREE free
#else
#define LIBFS_FREE(ptr)
#endif
#endif

#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#define __WINDOWS__
#endif

#ifdef __WINDOWS__
#define LIBFS_CDECL __cdecl
#define LIBFS_STDCALL __stdcall

/* export symbols by default, this is necessary for copy pasting the C and header file */
#if !defined(LIBFS_HIDE_SYMBOLS) && !defined(LIBFS_IMPORT_SYMBOLS) && !defined(LIBFS_EXPORT_SYMBOLS)
#define LIBFS_EXPORT_SYMBOLS
#endif

#if defined(LIBFS_HIDE_SYMBOLS)
#define LIBFS_PUBLIC(type)   type LIBFS_STDCALL
#elif defined(LIBFS_EXPORT_SYMBOLS)
#define LIBFS_PUBLIC(type)   __declspec(dllexport) type LIBFS_STDCALL
#elif defined(LIBFS_IMPORT_SYMBOLS)
#define LIBFS_PUBLIC(type)   __declspec(dllimport) type LIBFS_STDCALL
#endif
#else /* !__WINDOWS__ */
#define LIBFS_CDECL
#define LIBFS_STDCALL

#if (defined(__GNUC__) || defined(__SUNPRO_CC) || defined (__SUNPRO_C)) && defined(CJSON_API_VISIBILITY)
#define LIBFS_PUBLIC(type)   __attribute__((visibility("default"))) type
#else
#define LIBFS_PUBLIC(type) type
#endif
#endif

#include <stddef.h>

struct fs_hooks
{
    /* malloc/free are CDECL on Windows regardless of the default calling convention of the compiler, so ensure the hooks allow passing those functions directly. */
    void* (LIBFS_CDECL* malloc_fn)(size_t sz);
    void (LIBFS_CDECL* free_fn)(void* ptr);
};

LIBFS_PUBLIC(void) fs_init_hooks(struct fs_hooks* hooks);

/**
 * Composes an absolute path.
 * 
 * @code{.c}
 * char buf[MAX_PATH];
 * if (!fs_absolute("./relative", buf, MAX_PATH))
 * {
 *     print("fs_absolute failed");
 * }
 * else
 * {
 *     printf("%s", buf);
 * }
 * @endcode
 * 
 * @param[in] path Some null-terminated path
 * @param[out] buf Buffer for storing the result path
 * @param[in] size Buffer size
 * @return A pointer to buf if there is no error, NULL otherwise.
 */
LIBFS_PUBLIC(char*) fs_absolute(const char* path, char* buf, int size);

/**
 * Copies files or directories.
 * 
 * @code{.c}
 * fs_copy("foo.txt", "bar.txt");
 * @endcode 
 * 
 * @param[in] from Some null-terminated path to the source file, directory, or symlink
 * @param[in] to Some null-terminated path to the destination file, directory, or symlink
 */
LIBFS_PUBLIC(void) fs_copy(const char* from, const char* to);

/**
 * Copies file contents.
 * 
 * @code{.c}
 * fs_copy_file("foo.txt", "bar.txt");
 * @endcode 
 * 
 * @param[in] from Some null-terminated path to the source file
 * @param[in] to Some null-terminated path to the destination file
 */
LIBFS_PUBLIC(void) fs_copy_file(const char* from, const char* to);

/**
 * Get the current working directory.
 * 
 * @code{.c}
 * char buf[MAX_PATH];
 * if (!fs_current_path(buf, MAX_PATH))
 * {
 *     print("fs_current_path failed");
 * }
 * else
 * {
 *     printf("%s", buf);
 * }
 * @endcode 
 * 
 * @param[out] buf Buffer for storing the result path
 * @param[in] size Buffer size
 * @return A pointer to buf if there is no error, NULL otherwise.
 */
LIBFS_PUBLIC(char*) fs_current_path(char* buf, int size);

/**
 * Concatenates two paths together with the platform specific separator.
 * 
 * @code{.c}
 * char cwd[MAX_PATH];
 * if (!fs_current_path(cwd, MAX_PATH))
 * {
 *     print("fs_current_path failed");
 *     return;
 * }
 * 
 * char buf[MAX_PATH];
 * fs_join_path(buf, MAX_PATH, cwd, "foo.txt"); 
 * printf("%s", buf);
 * @endcode 
 * 
 * @param[out] buf Buffer for storing the result path
 * @param[in] size Buffer size
 * @param[in] left Left part null-terminated path
 * @param[in] right Right part null-terminated path
 * @return The number of bytes written to buf.
 */
LIBFS_PUBLIC(int) fs_join_path(char* buf, int size, const char* left, const char* right);

/**
 * Checks if a path corresponds to an existing file or directory.
 * 
 * @code{.c}
 * if (!fs_exists("./foo.txt"))
 * {
 *     print("foo.txt not found");
 * }
 * else
 * {
 *     printf("foo.txt found");
 * }
 * @endcode
 * 
 * @param[in] path Some null-terminated path
 * @return If the file or directory exists.
 */
LIBFS_PUBLIC(int) fs_exists(const char* path);

/**
 * Gets the size of an existing file.
 *
 * @code{.c}
 * long size = fs_file_size("foo.txt")
 * printf("file size: %d", size);
 * @endcode
 * 
 * @param[in] path Some null-terminated path
 * @return The size of the file, in bytes
 */
LIBFS_PUBLIC(long) fs_file_size(const char* path);

/**
 * Checks if a path corresponds to a directory.
 * 
 * @code{.c}
 * if (!fs_is_directory("./somedirectory"))
 * {
 *     print("path is not a directory");
 * }
 * else
 * {
 *     print("path is a directory");
 * }
 * @endcode 
 * 
 * @param[in] path Some null-terminated path
 * @return If path points to an existing directory.
 */
LIBFS_PUBLIC(int) fs_is_directory(const char* path);

/**
 * Checks if a path corresponds to a file.
 * 
 * @code{.c}
 * if (!fs_is_file("./foo.txt"))
 * {
 *     print("path is not a file");
 * }
 * else
 * {
 *     print("path is a file");
 * }
 * @endcode 
 * 
 * @param[in] path Some null-terminated path
 * @return If path points to an existing file.
 */
LIBFS_PUBLIC(int) fs_is_file(const char* path);

/**
 * Checks if a path corresponds to a symbolic link.
 * 
 * @code{.c}
 * if (!fs_is_symlink("./somesymlink"))
 * {
 *     print("path is not a symbolic link");
 * }
 * else
 * {
 *     print("path is a symbolic link");
 * }
 * @endcode 
 * 
 * @param[in] path Some null-terminated path
 * @return If path points to an existing symbolic link.
 */
LIBFS_PUBLIC(int) fs_is_symlink(const char* path);

/**
 * Reads a whole file content.
 * 
 * @code{.c}
 * int size;
 * void* buf;
 * if (!(buf = fs_read_file("foo.txt", &size)))
 * {
 *     printf("fs_read_file failed");
 * }
 * else
 * {
 *     printf("file size: %d", size);
 * }
 * @endcode 
 * 
 * @param[in] path Some null-terminated path to existing file
 * @param[out] size Number of bytes read
 * @return A pointer to read bytes if there is no error, NULL otherwise.
 */
LIBFS_PUBLIC(void*) fs_read_file(const char* path, int* size);

/**
 * Gets the absolute path to the platform specific temporary directory.
 *
 * @code{.c}
 * char buf[MAX_PATH];
 * if (!fs_temp_directory_path(buf, MAX_PATH))
 * {
 *     printf("fs_temp_directory_path failed");
 * }
 * else
 * {
 *     printf("%s", buf);
 * }
 * @endcode
 *
 * @param[in] buf Buffer for storing the result path
 * @param[in] size Buffer size
 * @return A pointer to buf if there is no error, NULL otherwise.
 */
LIBFS_PUBLIC(char*) fs_temp_directory_path(char* buf, int size);

/**
 * Struct used to iterate over a directory.
 *
 * @code{.c}
 * struct fs_directory_iterator* it = fs_open_dir("./somedir");
 * 
 * while(fs_read_dir(it))
 * {
 *     printf("%s", it->path);
 * }
 * 
 * fs_close_dir(it);
 * @endcode
 */
struct fs_directory_iterator
{
    /** Path to file. */
	const char* path;
};

/**
 * Gets an iterator over entries of a directory.
 *
 * @code{.c}
 * struct fs_directory_iterator* it = fs_open_dir("./somedir");
 * 
 * while(fs_read_dir(it))
 * {
 *     printf("%s", it->path);
 * }
 * 
 * fs_close_dir(it);
 * @endcode
 *
 * @param[in] path Some null-terminated path
 * @return A pointer for iterating over the directory if there is no error, NULL otherwise.
 */
LIBFS_PUBLIC(struct fs_directory_iterator*) fs_open_dir(const char* path);

/**
 * Iterates over the next entry of a directory.
 *
 * @code{.c}
 * struct fs_directory_iterator* it = fs_open_dir("./somedir");
 *
 * while(fs_read_dir(it))
 * {
 *     printf("%s", it->path);
 * }
 *
 * fs_close_dir(it);
 * @endcode
 *
 * @param[in] it Some opened directory iterator
 * @return The same it pointer or NULL if an error occurred or there is no more entry to iterate over.
 */
LIBFS_PUBLIC(struct fs_directory_iterator*) fs_read_dir(struct fs_directory_iterator* it);

/**
 * Closes and frees an opened directory iterator.
 *
 * @code{.c}
 * struct fs_directory_iterator* it = fs_open_dir("./somedir");
 *
 * while(fs_read_dir(it))
 * {
 *     printf("%s", it->path);
 * }
 *
 * fs_close_dir(it);
 * @endcode
 *
 * @param[in] it Some opened directory iterator
 */
LIBFS_PUBLIC(void) fs_close_dir(struct fs_directory_iterator* it);

#ifdef __cplusplus
}
#endif

#endif