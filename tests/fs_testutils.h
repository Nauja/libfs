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

static void _fs_exist(const char* path)
{
    assert_true(fs_exist(path));
}

static void _fs_non_exist(const char* path)
{
    assert_false(fs_exist(path));
}

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

static void _fs_make_dir(const char* path)
{
    assert_true(fs_make_dir(path));
}

static void _fs_delete_dir(const char* path)
{
    assert_true(fs_delete_dir(path));
}

static void _fs_delete_file(const char* path)
{
    assert_true(fs_delete_file(path));
}

static void* _fs_read_file(const char* path, size_t* size)
{
    void* buf = fs_read_file(path, size);
    assert_non_null(buf);
    return buf;
}

static void _fs_write_file(const char* path, const void* buf, size_t size)
{
    assert_true(fs_write_file(path, buf, size));
}

#ifdef LIBFS_DOXYGEN
/**
 * Asserts that fs_exist doesn't fail.
 * @param[in] path Some null-terminated path
 */
void fs_assert_exist(const char* path)
{
    _fs_exist(path);
}

/**
 * Asserts that fs_exist fails.
 * @param[in] path Some null-terminated path
 */
void fs_assert_non_exist(const char* path)
{
    _fs_non_exist(path);
}

/**
 * Asserts that fs_current_dir doesn't fail.
 * @param[out] buf Buffer for storing the result path
 */
void fs_assert_current_dir(char(*buf)[LIBFS_MAX_PATH])
{
    _fs_current_dir(buf);
}

/**
 * Asserts that fs_join_path doesn't fail.
 * @param[out] buf Buffer for storing the result path
 * @param[in] left Left part null-terminated path
 * @param[in] right Right part null-terminated path
 * @return The number of bytes written to buf.
 */
size_t fs_assert_join_path(char(*buf)[LIBFS_MAX_PATH], const char* left, const char* right)
{
    return _fs_join_path(buf, left, right);
}

/**
 * Asserts that fs_open_dir doesn't fail.
 * @param[in] path Some null-terminated path
 * @return A pointer for iterating over the directory.
 */
struct fs_directory_iterator* fs_assert_open_dir(const char* path)
{
    return _fs_open_dir(path);
}

/**
 * Asserts that fs_read_dir doesn't fail.
 * @param[in] it Some opened directory iterator
 * @return The same it pointer.
 */
struct fs_directory_iterator* fs_assert_read_dir(struct fs_directory_iterator* it)
{
    return _fs_read_dir(it);
}

/**
 * Asserts that fs_make_dir doesn't fail.
 * @param[in] path Some null-terminated path
 */
void fs_assert_make_dir(const char* path)
{
    _fs_make_dir(path);
}

/**
 * Asserts that fs_delete_dir doesn't fail.
 * @param[in] path Some null-terminated path
 */
void fs_assert_delete_dir(const char* path)
{
    _fs_delete_dir(path);
}

/**
 * Asserts that fs_delete_file doesn't fail.
 * @param[in] path Some null-terminated path
 */
void fs_assert_delete_file(const char* path)
{
    _fs_delete_file(path);
}

/**
 * Asserts that fs_read_dir doesn't fail.
 * @param[in] path Some null-terminated path to existing file
 * @param[out] size Number of bytes read
 * @return A pointer to read bytes.
 */
void* fs_assert_read_file(const char* path, size_t* size)
{
    return _fs_read_file(path, size);
}

/**
 * Asserts that fs_write_file doesn't fail.
 * @param[in] path Some null-terminated path to existing file
 * @param[out] buf Some buffer
 * @param[out] size Buffer size
 */
void fs_assert_write_file(const char* path, const void* buf, size_t size)
{
    _fs_write_file(path, buf, size);
}
#else
#define fs_assert_exist(path) _fs_exist(path)
#define fs_assert_non_exist(path) _fs_non_exist(path)
#define fs_assert_current_dir(buf) _fs_current_dir(buf)
#define fs_assert_join_path(buf, left, right) _fs_join_path(buf, left, right)
#define fs_assert_open_dir(path) _fs_open_dir(path)
#define fs_assert_read_dir(it) _fs_read_dir(it)
#define fs_assert_make_dir(path) _fs_make_dir(path)
#define fs_assert_delete_dir(path) _fs_delete_dir(path)
#define fs_assert_delete_file(path) _fs_delete_file(path)
#define fs_assert_read_file(path, size) _fs_read_file(path, size)
#define fs_assert_write_file(path, buf, size) _fs_write_file(path, buf, size)
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