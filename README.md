# libfs

[![CI](https://github.com/Nauja/libfs/actions/workflows/CI.yml/badge.svg)](https://github.com/Nauja/libfs/actions/workflows/CI.yml)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/Nauja/libfs/master/LICENSE)

Filesystem API in ANSI C.

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

## License

Licensed under the [MIT](LICENSE) License.
