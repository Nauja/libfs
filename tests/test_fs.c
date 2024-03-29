#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "fs_testutils.h"

#define DIRECTORY_DATA "data"
#define DIRECTORY_OUTPUT "output"
#define FILE_HELLO "data/hello.txt"
#define FILE_UNKNOWN "unknown"

typedef struct fs_file_iterator fs_file_iterator;
typedef struct fs_directory_iterator fs_directory_iterator;

static void test_rsplit_root(void **state)
{
    assert_null(fs_rsplit("foo.txt"));
}

static void test_rsplit_forwardslash(void **state)
{
    assert_string_equal(fs_rsplit("/path\\to/foo.txt"), "/foo.txt");
}

static void test_rsplit_backslash(void **state)
{
    assert_string_equal(fs_rsplit("/path/to\\foo.txt"), "\\foo.txt");
}

static void test_absolute(void **state)
{
    char cwd[256];
    fs_assert_current_dir(&cwd);

    char buf[LIBFS_MAX_PATH];
    fs_absolute(cwd, buf, LIBFS_MAX_PATH);

    assert_string_equal(cwd, buf);
}

static void test_dirname_root_file(void **state)
{
    char buf[LIBFS_MAX_PATH];
    fs_dirname("foo.txt", buf, LIBFS_MAX_PATH);
    assert_string_equal(buf, "");
}

static void test_dirname_dot(void **state)
{
    char buf[LIBFS_MAX_PATH];
    fs_dirname(".", buf, LIBFS_MAX_PATH);
    assert_string_equal(buf, "");
}

static void test_dirname_file(void **state)
{
    char buf[LIBFS_MAX_PATH];
    fs_dirname("/path/to\\bar.txt", buf, LIBFS_MAX_PATH);
    assert_string_equal(buf, "/path/to");
}

static void test_dirname_empty(void **state)
{
    char buf[LIBFS_MAX_PATH];
    fs_dirname("/foo/bar/", buf, LIBFS_MAX_PATH);
    assert_string_equal(buf, "/foo/bar");
}

static void test_basename_root_file(void **state)
{
    assert_string_equal(fs_basename("foo.txt"), "foo.txt");
}

static void test_basename_dot(void **state)
{
    assert_string_equal(fs_basename("."), ".");
}

static void test_basename_file(void **state)
{
    assert_string_equal(fs_basename("/path/to\\bar.txt"), "bar.txt");
}

static void test_basename_empty(void **state)
{
    assert_string_equal(fs_basename("/foo/bar/"), "");
}

static void test_get_cwd(void **state)
{
    char cwd[LIBFS_MAX_PATH];
    fs_assert_current_dir(&cwd);
    assert_true(fs_string_ends_with(cwd, "tests"));
}

static void test_path_join(void **state)
{
    char buf[LIBFS_MAX_PATH];
    fs_assert_join_path(&buf, "tests", "data");
    assert_string_equal(buf, "tests/data");
}

static void test_exists(void **state)
{
    char cwd[LIBFS_MAX_PATH];
    fs_assert_current_dir(&cwd);

    char buf[LIBFS_MAX_PATH];
    fs_assert_join_path(&buf, cwd, DIRECTORY_DATA);
    assert_true(fs_exist(buf));
    fs_assert_join_path(&buf, cwd, FILE_HELLO);
    assert_true(fs_exist(buf));
    fs_assert_join_path(&buf, cwd, FILE_UNKNOWN);
    assert_false(fs_exist(buf));
}

static void test_file_size(void **state)
{
    char cwd[LIBFS_MAX_PATH];
    fs_assert_current_dir(&cwd);

    char buf[LIBFS_MAX_PATH];
    fs_assert_join_path(&buf, cwd, DIRECTORY_DATA);
    assert_int_not_equal(fs_file_size(buf), 0);
    fs_assert_join_path(&buf, cwd, FILE_HELLO);
    assert_int_equal(fs_file_size(buf), 5);
    fs_assert_join_path(&buf, cwd, FILE_UNKNOWN);
    assert_int_equal(fs_file_size(buf), -1L);
}

static void test_is_directory(void **state)
{
    char cwd[LIBFS_MAX_PATH];
    fs_assert_current_dir(&cwd);

    char buf[LIBFS_MAX_PATH];
    fs_assert_join_path(&buf, cwd, DIRECTORY_DATA);
    assert_true(fs_is_directory(buf));
    fs_assert_join_path(&buf, cwd, FILE_HELLO);
    assert_false(fs_is_directory(buf));
    fs_assert_join_path(&buf, cwd, FILE_UNKNOWN);
    assert_false(fs_is_directory(buf));
}

static void test_is_file(void **state)
{
    char cwd[LIBFS_MAX_PATH];
    fs_assert_current_dir(&cwd);

    char buf[LIBFS_MAX_PATH];
    fs_assert_join_path(&buf, cwd, DIRECTORY_DATA);
    assert_false(fs_is_file(buf));
    fs_assert_join_path(&buf, cwd, FILE_HELLO);
    assert_true(fs_is_file(buf));
    fs_assert_join_path(&buf, cwd, FILE_UNKNOWN);
    assert_false(fs_is_file(buf));
}

static void test_read_unknown_file(void **state)
{
    char cwd[LIBFS_MAX_PATH];
    fs_assert_current_dir(&cwd);

    char buf[LIBFS_MAX_PATH];
    fs_assert_join_path(&buf, cwd, FILE_UNKNOWN);

    size_t size;
    void *data = fs_read_file(buf, &size);
    assert_null(data);
}

static void test_read_file_buffer(void **state)
{
    char cwd[LIBFS_MAX_PATH];
    fs_assert_current_dir(&cwd);

    char buf[LIBFS_MAX_PATH];
    fs_assert_join_path(&buf, cwd, FILE_HELLO);

    size_t size;
    char data[6];
    size = fs_read_file_buffer(buf, data, 6);
    assert_int_equal(size, 5);
    assert_string_equal(data, "hello");
    assert_int_equal((int)data[size], '\0');
}

static void test_read_file_buffer_too_big(void **state)
{
    char cwd[LIBFS_MAX_PATH];
    fs_assert_current_dir(&cwd);

    char buf[LIBFS_MAX_PATH];
    fs_assert_join_path(&buf, cwd, FILE_HELLO);

    size_t size;
    char data[10];
    size = fs_read_file_buffer(buf, data, 10);
    assert_int_equal(size, 5);
    assert_string_equal(data, "hello");
    assert_int_equal((int)data[5], '\0');
}

static void test_read_file_buffer_too_small(void **state)
{
    char cwd[LIBFS_MAX_PATH];
    fs_assert_current_dir(&cwd);

    char buf[LIBFS_MAX_PATH];
    fs_assert_join_path(&buf, cwd, FILE_HELLO);

    size_t size;
    char data[3];
    size = fs_read_file_buffer(buf, data, 3);
    assert_int_equal(size, 5);
    assert_string_equal(data, "he");
    assert_int_equal((int)data[2], '\0');
}

static void test_read_file(void **state)
{
    char cwd[LIBFS_MAX_PATH];
    fs_assert_current_dir(&cwd);

    char buf[LIBFS_MAX_PATH];
    fs_assert_join_path(&buf, cwd, FILE_HELLO);

    size_t size;
    char *data = (char *)fs_read_file(buf, &size);
    assert_non_null(data);
    assert_int_equal(size, 5);
    assert_string_equal(data, "hello");
    assert_int_equal((int)data[size], '\0');

    free(data);
}

static void test_iter_file(void **state)
{
    char cwd[LIBFS_MAX_PATH];
    fs_assert_current_dir(&cwd);

    char buf[LIBFS_MAX_PATH];
    fs_assert_join_path(&buf, cwd, FILE_HELLO);

    char expected[] = "hello";

    size_t i = 0;
    char c;
    fs_file_iterator *it = fs_assert_iter_file(buf);
    while (fs_next_char(it, &c))
    {
        assert_int_equal(c, expected[i]);
        ++i;
    }
    assert_int_equal(i, 5);
    fs_close_file(it);
}

static void test_read_dir(void **state)
{
    char cwd[LIBFS_MAX_PATH];
    fs_assert_current_dir(&cwd);

    fs_directory_iterator *it = fs_open_dir(cwd);
    assert_non_null(it);

    size_t has_data = 0;
    while (fs_read_dir(it))
    {
        if (strcmp(it->path, DIRECTORY_DATA) != 0)
        {
            has_data = 1;
        }
    }

    assert_true(has_data);

    fs_close_dir(it);
}

static void test_make_dir(void **state)
{
    char cwd[LIBFS_MAX_PATH];
    fs_assert_current_dir(&cwd);

    char buf[LIBFS_MAX_PATH];
    fs_assert_join_path(&buf, cwd, DIRECTORY_OUTPUT);

    fs_assert_delete_dir(buf);
    fs_assert_non_exist(buf);
    fs_assert_make_dir(buf);
    fs_assert_exist(buf);
    fs_assert_delete_dir(buf);
    fs_assert_non_exist(buf);
}

static void test_delete_file(void **state)
{
    char cwd[LIBFS_MAX_PATH];
    fs_assert_current_dir(&cwd);

    char buf[LIBFS_MAX_PATH];
    fs_assert_join_path(&buf, cwd, DIRECTORY_OUTPUT);

    fs_assert_make_dir(buf);
    fs_assert_exist(buf);

    char foo[LIBFS_MAX_PATH];
    fs_assert_join_path(&foo, buf, "foo.txt");

    const char *hello = "hello";
    fs_assert_write_file(foo, hello, 5);
    fs_assert_exist(foo);
    fs_assert_delete_file(foo);
    fs_assert_non_exist(foo);
}

static void test_read_unknown_dir(void **state)
{
    fs_directory_iterator *it = fs_open_dir("invalid dir");
    assert_null(it);
}

static void test_hooks(void **state)
{
    struct fs_hooks hooks = {malloc, free};
    fs_init_hooks(&hooks);
    test_read_dir(state);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_rsplit_root),
        cmocka_unit_test(test_rsplit_forwardslash),
        cmocka_unit_test(test_rsplit_backslash),
        cmocka_unit_test(test_absolute),
        cmocka_unit_test(test_dirname_root_file),
        cmocka_unit_test(test_dirname_dot),
        cmocka_unit_test(test_dirname_file),
        cmocka_unit_test(test_dirname_empty),
        cmocka_unit_test(test_basename_root_file),
        cmocka_unit_test(test_basename_dot),
        cmocka_unit_test(test_basename_file),
        cmocka_unit_test(test_basename_empty),
        cmocka_unit_test(test_get_cwd),
        cmocka_unit_test(test_path_join),
        cmocka_unit_test(test_exists),
        cmocka_unit_test(test_file_size),
        cmocka_unit_test(test_is_directory),
        cmocka_unit_test(test_is_file),
        cmocka_unit_test(test_read_unknown_file),
        cmocka_unit_test(test_read_file_buffer),
        cmocka_unit_test(test_read_file_buffer_too_big),
        cmocka_unit_test(test_read_file_buffer_too_small),
        cmocka_unit_test(test_read_file),
        cmocka_unit_test(test_iter_file),
        cmocka_unit_test(test_read_dir),
        cmocka_unit_test(test_make_dir),
        cmocka_unit_test(test_delete_file),
        cmocka_unit_test(test_read_unknown_dir),
        cmocka_unit_test(test_hooks)};
    return cmocka_run_group_tests(tests, NULL, NULL);
}