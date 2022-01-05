#ifndef LIBFS_TESTUTILS__h
#define LIBFS_TESTUTILS__h

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>
#include "fs.h"

#ifndef LIBFS_MAX_PATH
/** Maximum path size. */
#define LIBFS_MAX_PATH 256
#endif

static void _fs_current_dir(char (*buf)[LIBFS_MAX_PATH])
{
    assert_non_null(fs_current_dir(buf[0], LIBFS_MAX_PATH));
}

static size_t _fs_join_path(char(*buf)[LIBFS_MAX_PATH], const char* left, const char* right)
{
    size_t size = fs_join_path(buf[0], LIBFS_MAX_PATH, left, right);
    assert_true(size);
    return size;
}

static struct fs_directory_iterator* _fs_open_dir(const char* path)
{
    struct fs_directory_iterator* it = fs_open_dir(path);
    assert_non_null(it);
    return it;
}

static struct fs_directory_iterator* _fs_read_dir(struct fs_directory_iterator* it)
{
    it = fs_read_dir(it);
    assert_non_null(it);
    return it;
}

static void* _fs_read_file(const char* path, size_t* size)
{
    void* buf = fs_read_file(path, size);
    assert_non_null(buf);
    return buf;
}

#ifdef LIBFS_DOXYGEN
/**
 * Asserts that fs_current_dir doesn't fail.
 * @param[out] buf Buffer for storing the result path
 */
void fs_assert_current_dir(char(*buf)[LIBFS_MAX_PATH]);

/**
 * Asserts that fs_join_path doesn't fail.
 * @param[out] buf Buffer for storing the result path
 * @param[in] left Left part null-terminated path
 * @param[in] right Right part null-terminated path
 * @return The number of bytes written to buf.
 */
size_t fs_assert_join_path(char(*buf)[LIBFS_MAX_PATH], const char* left, const char* right);

/**
 * Asserts that fs_open_dir doesn't fail.
 * @param[in] path Some null-terminated path
 * @return A pointer for iterating over the directory.
 */
struct fs_directory_iterator* fs_assert_open_dir(const char* path);

/**
 * Asserts that fs_read_dir doesn't fail.
 * @param[in] it Some opened directory iterator
 * @return The same it pointer.
 */
struct fs_directory_iterator* fs_assert_read_dir(struct fs_directory_iterator* it);

/**
 * Asserts that fs_read_dir doesn't fail.
 * @param[in] path Some null-terminated path to existing file
 * @param[out] size Number of bytes read
 * @return A pointer to read bytes.
 */
void* fs_assert_read_file(const char* path, size_t* size);
#else
#define fs_assert_current_dir(buf) _fs_current_dir(buf)
#define fs_assert_join_path(buf, left, right) _fs_join_path(buf, left, right)
#define fs_assert_open_dir(path) _fs_open_dir(path)
#define fs_assert_read_dir(it) _fs_read_dir(it)
#define fs_assert_read_file(path, size) _fs_read_file(path, size)
#endif

/**
 * Checks that a string ends with a suffix.
 * @param[in] str Some null-terminated string
 * @param[in] suffix Some null-terminated string
 * @return If str ends with suffix.
 */
int fs_string_ends_with(const char* str, const char* suffix)
{
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    return
        (str_len >= suffix_len) &&
        (0 == strcmp(str + (str_len - suffix_len), suffix));
}

#ifdef __cplusplus
}
#endif

#endif