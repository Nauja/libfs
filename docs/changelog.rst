.. -*- coding: utf-8 -*-
.. _changelog:

=========
Changelog
=========

v0.2.0 (Dec 21, 2022)
---------------------

  * Improve CMakeLists.txt
  * Improve documentation
  * Add workflows for Windows/Mac/Linux
  * Add missing LIBFS_PUBLIC in fs.c
  * Fix many warnings
  * Add fs_iter_file fs_next_char fs_close_file fs_assert_iter_file

v0.1.2 (Jan 13, 2022)
---------------------

  * Rename:
  
    * LIBFS_STATIC_LIB to LIBFS_STATIC
    * UNIT_TESTING to LIBFS_UNIT_TESTING
    * DOXYGEN to LIBFS_DOXYGEN

v0.1.1 (Jan 7, 2022)
--------------------

  * Add fs_make_dir fs_delete_dir fs_write_file fs_delete_file
  * Rename fs_exists to fs_exist
  * Add a test API
  * Fix missing null-terminating character with fs_read_file
  * More documentation

v0.1.0 (Jan 4, 2022)
--------------------

  * Stable API
  * Continuous integration
  * Doxygen + Sphinx documentation
