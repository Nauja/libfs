#pragma once
#include <cmocka.h>
#include <string.h>
#include "fs.h"

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

static void _get_cwd(char* buf)
{
	assert_non_null(fs_get_cwd(buf, MAX_PATH));
}

static int _join_path(char* buf, const char* left, const char* right)
{
	int size = fs_join_path(buf, MAX_PATH, left, right);
	assert_true(size);
	return size;
}

#define assert_get_cwd(buf) _get_cwd(buf)
#define assert_join_path(buf, left, right) _join_path(buf, left, right)

int string_ends_with(const char* str, const char* suffix)
{
    int str_len = strlen(str);
    int suffix_len = strlen(suffix);

    return
        (str_len >= suffix_len) &&
        (0 == strcmp(str + (str_len - suffix_len), suffix));
}