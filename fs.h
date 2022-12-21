#ifndef LIBFS__h
#define LIBFS__h

#ifdef __cplusplus
extern "C"
{
#endif

/** Major version of libfs. */
#define LIBFS_VERSION_MAJOR 0
/** Minor version of libfs. */
#define LIBFS_VERSION_MINOR 2
/** Patch version of libfs. */
#define LIBFS_VERSION_PATCH 0

/* Define to 1 if you have the <dirent.h> header file. */
#ifndef HAVE_DIRENT_H
#define HAVE_DIRENT_H 1
#endif

/* Define to 1 if you have the <stddef.h> header file. */
#ifndef HAVE_STDDEF_H
#define HAVE_STDDEF_H 1
#endif

/* Define to 1 if you have the <stdio.h> header file. */
#ifndef HAVE_STDIO_H
#define HAVE_STDIO_H 1
#endif

/* Define to 1 if you have the <stdlib.h> header file. */
#ifndef HAVE_STDLIB_H
#define HAVE_STDLIB_H 1
#endif

/* Define to 1 if you have the <sys/stat.h> header file. */
#ifndef HAVE_SYS_STAT_H
#define HAVE_SYS_STAT_H 1
#endif

/* Define to 1 if you have the <sys/sendfile.h> header file. */
#ifndef HAVE_SYS_SENDFILE_H
#define HAVE_SYS_SENDFILE_H 1
#endif

/* Define to 1 if you have the <string.h> header file. */
#ifndef HAVE_STRING_H
#define HAVE_STRING_H 1
#endif

/* Define to 1 if you have the <unistd.h> header file. */
#ifndef HAVE_UNISTD_H
#define HAVE_UNISTD_H 1
#endif

/* Define to 1 if you have the <windows.h> header file. */
#ifndef HAVE_WINDOWS_H
/* #undef HAVE_WINDOWS_H */
#endif

/* Define to 1 if you have the `free' function. */
#ifndef HAVE_FREE
#define HAVE_FREE 1
#endif

/* Define to 1 if you have the `malloc' function. */
#ifndef HAVE_MALLOC
#define HAVE_MALLOC 1
#endif

/* Define to 1 if you have the `memset' function. */
#ifndef HAVE_MEMSET
#define HAVE_MEMSET 1
#endif

/* Define to 1 if you have the `memcpy' function. */
#ifndef HAVE_MEMCPY
#define HAVE_MEMCPY 1
#endif

/* Define to 1 if you have the `_snprintf_s' function. */
#ifndef HAVE__SNPRINTF_S
/* #undef HAVE__SNPRINTF_S */
#endif

/* Define to 1 if you have the `_snprintf' function. */
#ifndef HAVE__SNPRINTF
/* #undef HAVE__SNPRINTF */
#endif

/* Define to 1 if you have the `snprintf' function. */
#ifndef HAVE_SNPRINTF
#define HAVE_SNPRINTF 1
#endif

/* Define to 1 if you have the `vsnprintf' function. */
#ifndef HAVE_VSNPRINTF
#define HAVE_VSNPRINTF 1
#endif

/* Define to 1 if you build with Doxygen. */
#ifndef LIBFS_DOXYGEN
/* #undef LIBFS_DOXYGEN */
#endif

#ifdef HAVE_STDDEF_H
/* Required for size_t */
#include <stddef.h>
#endif

#ifndef LIBFS_MALLOC
#ifdef HAVE_MALLOC
/**
 * Defines the malloc function used by libfs at compile time.
 *
 * @code
 * void* my_malloc(size_t size)
 * {
 *     // do something
 * }
 *
 * #define LIBFS_MALLOC my_malloc
 * @endcode
 */
#define LIBFS_MALLOC malloc
#else
#define LIBFS_MALLOC(size) NULL
#endif
#endif

#ifndef LIBFS_FREE
#ifdef HAVE_FREE
/**
 * Defines the free function used by libfs at compile time.
 *
 * @code
 * void my_free(void* ptr)
 * {
 *     // do something
 * }
 *
 * #define LIBFS_FREE my_free
 * @endcode
 */
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
#define LIBFS_PUBLIC(type) type LIBFS_STDCALL
#elif defined(LIBFS_EXPORT_SYMBOLS)
#define LIBFS_PUBLIC(type) __declspec(dllexport) type LIBFS_STDCALL
#elif defined(LIBFS_IMPORT_SYMBOLS)
#define LIBFS_PUBLIC(type) __declspec(dllimport) type LIBFS_STDCALL
#endif
#else /* !__WINDOWS__ */
#define LIBFS_CDECL
#define LIBFS_STDCALL

#if (defined(__GNUC__) || defined(__SUNPRO_CC) || defined(__SUNPRO_C)) && defined(CJSON_API_VISIBILITY)
#define LIBFS_PUBLIC(type) __attribute__((visibility("default"))) type
#else
#define LIBFS_PUBLIC(type) type
#endif
#endif

    /** Struct for custom hooks configuration. */
    struct fs_hooks
    {
        /** Custom malloc function. */
        void *(LIBFS_CDECL *malloc_fn)(size_t size);

        /**  Custom free function. */
        void(LIBFS_CDECL *free_fn)(void *ptr);
    };

    /**
     * Register custom hooks.
     *
     * @code{.c}
     * struct fs_hooks hooks = { malloc, free };
     * fs_init_hooks(&hooks);
     * @endcode
     *
     * @param[in] hooks Hooks configuration
     */
    LIBFS_PUBLIC(void)
    fs_init_hooks(struct fs_hooks *hooks);

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
    LIBFS_PUBLIC(char *)
    fs_absolute(const char *path, char *buf, size_t size);

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
    LIBFS_PUBLIC(void)
    fs_copy(const char *from, const char *to);

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
    LIBFS_PUBLIC(void)
    fs_copy_file(const char *from, const char *to);

    /**
     * Get the current working directory.
     *
     * @code{.c}
     * char buf[MAX_PATH];
     * if (!fs_current_dir(buf, MAX_PATH))
     * {
     *     print("fs_current_dir failed");
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
    LIBFS_PUBLIC(char *)
    fs_current_dir(char *buf, size_t size);

    /**
     * Concatenates two paths together with the platform specific separator.
     *
     * @code{.c}
     * char cwd[MAX_PATH];
     * if (!fs_current_dir(cwd, MAX_PATH))
     * {
     *     print("fs_current_dir failed");
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
    LIBFS_PUBLIC(size_t)
    fs_join_path(char *buf, size_t size, const char *left, const char *right);

    /**
     * Checks if a path corresponds to an existing file or directory.
     *
     * @code{.c}
     * if (!fs_exist("./foo.txt"))
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
    LIBFS_PUBLIC(int)
    fs_exist(const char *path);

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
    LIBFS_PUBLIC(size_t)
    fs_file_size(const char *path);

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
    LIBFS_PUBLIC(int)
    fs_is_directory(const char *path);

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
    LIBFS_PUBLIC(int)
    fs_is_file(const char *path);

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
    LIBFS_PUBLIC(int)
    fs_is_symlink(const char *path);

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
    LIBFS_PUBLIC(void *)
    fs_read_file(const char *path, size_t *size);

    /**
     * Writes content to file.
     *
     * @code{.c}
     * const char* buf = "hello";
     * if (!fs_write_file("foo.txt", buf, 5))
     * {
     *     printf("fs_write_file failed");
     * }
     * @endcode
     *
     * @param[in] path Some null-terminated path
     * @param[in] buf Some memory buffer
     * @param[in] size Buffer size
     * @return If the file was written.
     */
    LIBFS_PUBLIC(int)
    fs_write_file(const char *path, const void *buf, size_t size);

    /**
     * @struct fs_file_iterator
     * Struct used to iterate over a file.
     *
     * @code{.c}
     * struct fs_file_iterator* it = fs_iter_file("foo.txt");
     * char c;
     *
     * while(fs_next_char(it, &c))
     * {
     *     printf("%c", c);
     * }
     *
     * fs_close_file(it);
     * @endcode
     */
    struct fs_file_iterator;

    /**
     * Opens a file to iterate over its content.
     *
     * @code{.c}
     * struct fs_file_iterator* it = fs_iter_file("foo.txt");
     *
     * // iterate file
     *
     * fs_close_file(it);
     * @endcode
     *
     * @param[in] path Some null-terminated path
     * @return A pointer for iterating over the file if there is no error, NULL otherwise.
     */
    LIBFS_PUBLIC(struct fs_file_iterator *)
    fs_iter_file(const char *path);

    /**
     * Iterates over the next char of a file.
     *
     * @code{.c}
     * char c;
     * while(fs_next_char(it, &c))
     * {
     *     printf("%c", c);
     * }
     * @endcode
     *
     * @param[in] it Some opened file iterator
     * @param[out] c Character read
     * @return The same it pointer or NULL if an error occurred or there is no more entry to iterate over.
     */
    LIBFS_PUBLIC(struct fs_file_iterator *)
    fs_next_char(struct fs_file_iterator *it, char *c);

    /**
     * Closes and frees an opened file iterator.
     *
     * @code{.c}
     * struct fs_file_iterator* it = fs_iter_file("foo.txt");
     *
     * // iterate file
     *
     * fs_close_file(it);
     * @endcode
     *
     * @param[in] it Some opened file iterator
     */
    LIBFS_PUBLIC(void)
    fs_close_file(struct fs_file_iterator *it);

    /**
     * Gets the absolute path to the platform specific temporary directory.
     *
     * @code{.c}
     * char buf[MAX_PATH];
     * if (!fs_temp_dir(buf, MAX_PATH))
     * {
     *     printf("fs_temp_dir failed");
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
    LIBFS_PUBLIC(char *)
    fs_temp_dir(char *buf, size_t size);

    /**
     * Deletes an empty directory if it exists.
     *
     * @code
     * if (!fs_delete_dir("foo"))
     * {
     *     printf("fs_delete_dir failed");
     * }
     * @endcode
     * @param[in] path Some null-terminated path
     * @return If the directory was deleted.
     */
    LIBFS_PUBLIC(int)
    fs_delete_dir(const char *path);

    /**
     * Deletes a file if it exists.
     *
     * @code
     * if (!fs_delete_file("foo.txt"))
     * {
     *     printf("fs_delete_file failed");
     * }
     * @endcode
     * @param[in] path Some null-terminated path
     * @return If the file was deleted.
     */
    LIBFS_PUBLIC(int)
    fs_delete_file(const char *path);

    /**
     * Creates a directory if it doesn't exist.
     *
     * The parent directory must exist as it will not
     * be created recursively.
     *
     * @code
     * if (!fs_make_dir("foo"))
     * {
     *     printf("fs_make_dir failed");
     * }
     * @endcode
     * @param[in] path Some null-terminated path
     * @return If the directory was created.
     */
    LIBFS_PUBLIC(int)
    fs_make_dir(const char *path);

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
        const char *path;
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
    LIBFS_PUBLIC(struct fs_directory_iterator *)
    fs_open_dir(const char *path);

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
    LIBFS_PUBLIC(struct fs_directory_iterator *)
    fs_read_dir(struct fs_directory_iterator *it);

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
    LIBFS_PUBLIC(void)
    fs_close_dir(struct fs_directory_iterator *it);

#ifdef __cplusplus
}
#endif

#endif
