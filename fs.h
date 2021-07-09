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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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
 * Get the current working directory
 * @param buf buffer for writing path
 * @param size buffer size
 * @return buffer or NULL
 */
LIBFS_PUBLIC(char*) fs_get_cwd(char* buf, int size);

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
 * Tell if a path points to an existing directory
 * @param path path to test
 * @return if path is a directory
 */
LIBFS_PUBLIC(int) fs_is_directory(const char* path);

/**
 * Tell if a path points to an existing file
 * @param path path to test
 * @return if path is a file
 */
LIBFS_PUBLIC(int) fs_is_file(const char* path);

/**
 * Read content of a file
 * @param path path to file
 * @param size readen size
 * @return file content
 */
LIBFS_PUBLIC(void*) fs_read_file(const char* path, int* size);

#ifdef __cplusplus
}
#endif

#endif