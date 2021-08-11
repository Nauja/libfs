# libfs

[![CI](https://github.com/Nauja/libfs/actions/workflows/CI.yml/badge.svg)](https://github.com/Nauja/libfs/actions/workflows/CI.yml)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/Nauja/libfs/master/LICENSE)

Portable filesystem API in ANSI C.

## Build with CMake

```
git clone https://github.com/Nauja/libfs.git
cd libfs
git submodule init
git submodule update
mkdir build
cd build
cmake .. || (rm -rf * && cmake ..)
cmake --build .
make
```

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
```

## License

Licensed under the [MIT](LICENSE) License.
