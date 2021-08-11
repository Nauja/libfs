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

/* Define to 1 if you have the <stdio.h> header file. */
#cmakedefine HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#cmakedefine HAVE_STDLIB_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#cmakedefine HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/sendfile.h> header file. */
#cmakedefine HAVE_SYS_SENDFILE_H 1

/* Define to 1 if you have the <string.h> header file. */
#cmakedefine HAVE_STRING_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#cmakedefine HAVE_UNISTD_H 1

/* Define to 1 if you have the <windows.h> header file. */
#cmakedefine HAVE_WINDOWS_H 1

/* Define to 1 if you have the `free' function. */
#cmakedefine HAVE_FREE 1

/* Define to 1 if you have the `malloc' function. */
#cmakedefine HAVE_MALLOC 1

/* Define to 1 if you have the `memset' function. */
#cmakedefine HAVE_MEMSET 1

/* Define to 1 if you have the `memcpy' function. */
#cmakedefine HAVE_MEMCPY 1

/* Define to 1 if you have the `_snprintf_s' function. */
#cmakedefine HAVE__SNPRINTF_S 1

/* Define to 1 if you have the `_snprintf' function. */
#cmakedefine HAVE__SNPRINTF 1

/* Define to 1 if you have the `snprintf' function. */
#cmakedefine HAVE_SNPRINTF 1

/* Define to 1 if you have the `vsnprintf' function. */
#cmakedefine HAVE_VSNPRINTF 1

#ifndef LIBFS_MALLOC
#ifdef HAVE_MALLOC
#define LIBFS_MALLOC malloc
#endif
#endif

#ifndef LIBFS_FREE
#ifdef HAVE_FREE
#define LIBFS_FREE free
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

/**
 * Composes an absolute path
 * @param buf buffer for writing path
 * @param size buffer size
 * @return buffer or NULL
 */
LIBFS_PUBLIC(void) fs_absolute(const char* path, char* buf, int size);

/**
 * Copies files or directories
 * @param from path to the source file, directory, or symlink
 * @param to path to the target file, directory, or symlink
 */
LIBFS_PUBLIC(void) fs_copy(const char* from, const char* to);

/**
 * Copies file contents
 * @param from path to the source file
 * @param to path to the target file
 */
LIBFS_PUBLIC(void) fs_copy_file(const char* from, const char* to);

/**
 * Get the current working directory
 * @param buf buffer for writing path
 * @param size buffer size
 * @return buffer or NULL
 */
LIBFS_PUBLIC(char*) fs_current_path(char* buf, int size);

/**
 * Concatenate two paths together
 * @param buf buffer
 * @param size buffer size
 * @param left first path
 * @param right second path
 * @return number of bytes written
 */
LIBFS_PUBLIC(int) fs_join_path(char* buf, int size, const char* left, const char* right);

/**
 * Checks if the given path corresponds to an existing file or directory
 * @param path path to test
 * @return if path exists
 */
LIBFS_PUBLIC(int) fs_exists(const char* path);

/**
 * Returns the size of a file
 * @param path path to examine
 * @return the size of the file, in bytes
 */
LIBFS_PUBLIC(long) fs_file_size(const char* path);

/**
 * Checks if the given path corresponds to a directory
 * @param path path to test
 * @return if path is a directory
 */
LIBFS_PUBLIC(int) fs_is_directory(const char* path);

/**
 * Checks if the given path corresponds to a file
 * @param path path to test
 * @return if path is a file
 */
LIBFS_PUBLIC(int) fs_is_file(const char* path);

/**
 * Checks if the given path corresponds to a symbolic link
 * @param path path to test
 * @return if path is a symbolic link
 */
LIBFS_PUBLIC(int) fs_is_symlink(const char* path);

/**
 * Read content of a file
 * @param path path to file
 * @param size readen size
 * @return file content
 */
LIBFS_PUBLIC(void*) fs_read_file(const char* path, int* size);

LIBFS_PUBLIC(void) fs_temp_directory_path(char* buf, int size);

#ifdef __cplusplus
}
#endif

#endif