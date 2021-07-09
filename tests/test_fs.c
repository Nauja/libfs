#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "test_config.h"
#include "fs.h"

#define DIRECTORY_DATA "data"
#define FILE_HELLO "data/hello"
#define FILE_UNKNOWN "unknown"

static void test_get_cwd(void** state) {
    char cwd[MAX_PATH];
    assert_get_cwd(cwd);
    assert_true(string_ends_with(cwd, "tests"));
}

static void test_path_join(void** state) {
    char buf[MAX_PATH];
    assert_join_path(buf, "tests", "data");
    assert_true(strcmp(buf, "tests/data") == 0);
}

static void test_is_directory(void** state) {
    char cwd[MAX_PATH];
    assert_get_cwd(cwd);

    char buf[MAX_PATH];
    assert_join_path(buf, cwd, DIRECTORY_DATA);
    assert_true(fs_is_directory(buf));
    assert_join_path(buf, cwd, FILE_HELLO);
    assert_false(fs_is_directory(buf));
    assert_join_path(buf, cwd, FILE_UNKNOWN);
    assert_false(fs_is_directory(buf));
}

static void test_is_file(void** state) {
    char cwd[MAX_PATH];
    assert_get_cwd(cwd);

    char buf[MAX_PATH];
    assert_join_path(buf, cwd, DIRECTORY_DATA);
    assert_false(fs_is_file(buf));
    assert_join_path(buf, cwd, FILE_HELLO);
    assert_true(fs_is_file(buf));
    assert_join_path(buf, cwd, FILE_UNKNOWN);
    assert_false(fs_is_file(buf));
}

static void test_read_unknown_file(void** state) {
    char cwd[MAX_PATH];
    assert_get_cwd(cwd);

    char buf[MAX_PATH];
    assert_join_path(buf, cwd, FILE_UNKNOWN);

    int size;
    void* data = fs_read_file(buf, &size);
    assert_null(data);
}

static void test_read_file(void** state) {
    char cwd[MAX_PATH];
    assert_get_cwd(cwd);

    char buf[MAX_PATH];
    assert_join_path(buf, cwd, FILE_HELLO);

    int size;
    void* data = fs_read_file(buf, &size);
    assert_non_null(data);

    free(data);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_get_cwd),
        cmocka_unit_test(test_path_join),
        cmocka_unit_test(test_is_directory),
        cmocka_unit_test(test_is_file),
        cmocka_unit_test(test_read_unknown_file),
        cmocka_unit_test(test_read_file)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}