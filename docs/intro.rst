.. -*- coding: utf-8 -*-
.. currentmodule:: libfs

==============
 Introduction
==============

Portable filesystem API in ANSI C.

Build Manually
--------------

Copy the files fs.c and fs.h into an existing project.

Comment or uncomment the defines at the top of fs.h depending on your configuration:

.. code-block:: c

    /* Define to 1 if you have the <stdio.h> header file. */
    #define HAVE_STDIO_H 1

    /* Define to 1 if you have the <stdlib.h> header file. */
    #define HAVE_STDLIB_H 1

    /* Define to 1 if you have the <sys/stat.h> header file. */
    #define HAVE_SYS_STAT_H 1

    ...

You should now be able to compile this library correctly.

Build with CMake
-----------------

Tested with CMake >= 3.13.4:

.. code-block::

    git clone https://github.com/Nauja/libfs.git
    cd libfs
    git submodule init
    git submodule update
    mkdir build
    cd build
    cmake ..

CMake will correctly configure the defines at the top of fs.h for your system.

You can then build this library manually as described above, or by using:

.. code-block::
     
    make

This will generate libfs.a if building as a static library and liblibfs.so in the build directory.

You can change the build process with a list of different options that you can pass to CMake. Turn them on with On and off with Off:

    * -DWITH_STATIC_LIB=On: Enable building as static library. (on by default)
    * -DUNIT_TESTING=On: Enable building the tests. (on by default)

Build with Visual Studio
------------------------

Generate the Visual Studio solution with:

.. code-block::

    mkdir build
    cd build
    cmake .. -G "Visual Studio 16 2019"

You can now open build/libfs.sln and compile the library.

Usage
-----

Get the current working directory:

.. code-block:: c

    char cwd[MAX_PATH];
    fs_current_dir(&cwd, MAX_PATH);

Check if a file exists:

.. code-block:: c

    char buf[MAX_PATH];
    fs_join_path(&buf, MAX_PATH, cwd, "foo.txt");
    if (!fs_exists(buf)) {
        printf("file not found: %s\n", buf);
    }

Read whole file content:

.. code-block:: c

    int size;
    void* data = fs_read_file(buf, &size);
    if (data == NULL) {
        printf("can't read file");
    } else {
        printf("file size: %d\n", size);
    }

    free(data);

License
-------

Licensed under the MIT License.