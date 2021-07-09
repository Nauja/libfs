include(CheckIncludeFile)
include(CheckFunctionExists)

# HEADER FILES
check_include_file(malloc.h HAVE_MALLOC_H)
check_include_file(unistd.h HAVE_UNISTD_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(stdio.h HAVE_STDIO_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(string.h HAVE_STRING_H)
check_include_file(windows.h HAVE_WINDOWS_H)

# FUNCTIONS
check_function_exists(free HAVE_FREE)
check_function_exists(malloc HAVE_MALLOC)
check_function_exists(memset HAVE_MEMSET)
check_function_exists(memcpy HAVE_MEMCPY)