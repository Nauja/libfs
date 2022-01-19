.. -*- coding: utf-8 -*-
.. _changelog:

=========
Changelog
=========

.. module:: libfs

v0.1.3
------

  * Better organize doc
  * Fixed a warning with implicit cast from fs_posix_directory_iterator to fs_directory_iterator
  * Add missing stdarg.h include before cmocka.h

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
