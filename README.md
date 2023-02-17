# libfs

[![CI](https://github.com/Nauja/libfs/actions/workflows/CI.yml/badge.svg)](https://github.com/Nauja/libfs/actions/workflows/CI.yml)
[![CI Docs](https://github.com/Nauja/libfs/actions/workflows/CI_docs.yml/badge.svg)](https://github.com/Nauja/libfs/actions/workflows/CI_docs.yml)
[![Documentation Status](https://readthedocs.org/projects/libfs/badge/?version=latest)](https://libfs.readthedocs.io/en/latest/?badge=latest)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/Nauja/libfs/master/LICENSE)

libfs aims to be a portable (OSX/Linux/Windows/Emscripten) and lightweight filesystem API written in ANSI C.

## Examples

Get the current working directory:

```c
char cwd[MAX_PATH];
fs_current_dir(&cwd, MAX_PATH);
```

Check if a file exists:

```c
char buf[MAX_PATH];
fs_join_path(&buf, MAX_PATH, cwd, "foo.txt");
if (!fs_exist(buf))
{
    printf("file not found: %s\n", buf);
}
```

Read whole file content:

```c
int size;
void* data = fs_read_file(buf, &size);
if (data == NULL)
{
    printf("can't read file");
}
else
{
    printf("file size: %d\n", size);
}

free(data);
```

List a directory:

```c
struct fs_directory_iterator* it = fs_open_dir(cwd);

while(fs_read_dir(it))
{
    prinf("%s", it->path);
}

fs_close_dir(it);
```

Check the [documentation](https://libfs.readthedocs.io/en/latest/) to find more examples and learn about the API.

## Build Manually

Copy the files [fs.c](https://github.com/Nauja/libfs/blob/main/fs.c) and [fs.h](https://github.com/Nauja/libfs/blob/main/fs.h) into an existing project.

Comment or uncomment the defines at the top of `fs.h` depending on your configuration:

```c
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

...
```

You should now be able to compile this library correctly.

## Build with CMake

Tested with CMake >= 3.13.4:

```
git clone https://github.com/Nauja/libfs.git
cd libfs
git submodule init
git submodule update
mkdir build
cd build
cmake ..
```

CMake will correctly configure the defines at the top of [fs.h](https://github.com/Nauja/libfs/blob/main/fs.h) for your system.

You can then build this library manually as described above, or by using:

```
cmake --build .
```

This will generate `libfs.a` if building as a static library and `liblibfs.so` in the `build` directory.

You can change the build process with a list of different options that you can pass to CMake. Turn them on with `On` and off with `Off`:
  * `-DLIBFS_STATIC=On`: Enable building as static library. (on by default)
  * `-DLIBFS_SHARED=On`: Enable building as shared library. (on by default)
  * `-DLIBFS_UNIT_TESTING=On`: Enable building the tests. (on by default)
  * `-DLIBFS_DOXYGEN=On`: Enable building the docs. (off by default)

## Build with Emscripten

Tested with emsdk == 3.1.31:

```
git clone https://github.com/Nauja/libfs.git
cd libfs
git submodule init
git submodule update
mkdir build
cd build
emcmake cmake ..
```

You can then build this library by using:

```
cmake --build .
```

This will generate `libfs.js` and `libfs.wasm` in the `build` directory.

## Build with Visual Studio

Generate the Visual Studio solution with:

```
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
```

You can now open `build/libfs.sln` and compile the library.

## License

Licensed under the [MIT](https://github.com/Nauja/libfs/blob/main/LICENSE) License.
