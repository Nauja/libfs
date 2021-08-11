# libfs

[![CI](https://github.com/Nauja/libfs/actions/workflows/CI.yml/badge.svg)](https://github.com/Nauja/libfs/actions/workflows/CI.yml)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/Nauja/libfs/master/LICENSE)

Portable filesystem API in ANSI C.

## Build Manually

Copy the files [fs.c](fs.c) and [fs.h](fs.h) into an existing project.

Comment or uncomment the defines at the top of `fs.h` depending on your configuration:

```c
/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

...
```

You should now be able to compile this library correctly.

## Build with CMake

```
git clone https://github.com/Nauja/libfs.git
cd libfs
git submodule init
git submodule update
mkdir build
cd build
cmake ..
make
```

You can change the build process with a list of different options that you can pass to CMake. Turn them on with `On` and off with `Off`:
  * `-DWITH_STATIC_LIB=On`: Enable building as static library. (on by default)
  * `-DUNIT_TESTING=On`: Enable building the tests. (on by default)

## Usage

Get the current working directory:

```c
char cwd[MAX_PATH];
fs_current_path(&cwd, MAX_PATH);
```

Check if a file exists:

```c
char buf[MAX_PATH];
fs_join_path(&buf, MAX_PATH, cwd, "foo.txt");
if (!fs_exists(buf)) {
  printf("file not found: %s\n", buf);
}
```

Read whole file content:

```c
int size;
void* data = fs_read_file(buf, &size);
if (data == NULL) {
  printf("can't read file");
} else {
  printf("file size: %d\n", size);
}

free(data);
```

## License

Licensed under the [MIT](LICENSE) License.
