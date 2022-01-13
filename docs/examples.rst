.. -*- coding: utf-8 -*-
.. _examples:

===============
Advanced Usages
===============

.. module:: libfs

Those are more advanced usages for experienced users.

Build as a Static Library with CMake
------------------------------------

You can build libfs as a static library by passing ``-DLIBFS_STATIC=true`` as an argument to cmake:

.. code-block::

    mkdir build
    cd build
    cmake .. -DLIBFS_STATIC=true
    cmake --build .
    make

This will generate ``libfs.a`` in the ``build`` directory.

Build the Documentation with CMake
----------------------------------

To build the documentation you will first have to install `Doxygen <https://www.doxygen.nl>`_ and `Python <https://www.python.org/>`_:

.. code-block::

    apt-get install doxygen graphviz python3 python3-pip

On Windows, install them via the binaries.

Then do the following commands to install `Sphinx <https://www.sphinx-doc.org/en/master/usage/installation.html>`_ and its dependencies:

.. code-block::

    python3 -m pip install sphinx-build
    python3 -m pip install -r docs/requirements.txt

Now you can build the documentation with:

.. code-block::

    mkdir build
    cd build
    cmake .. -DLIBFS_DOXYGEN=true
    cmake --build .
    make

The documentation is generated in ``build/docs/sphinx/index.html``.

Custom malloc/free with Defines
-------------------------------

You can define the two macros ``LIBFS_MALLOC`` and ``LIBFS_FREE`` in your code to
provide custom malloc or free functions at compile time for custom memory management:

.. code-block:: c

    void* my_malloc(size_t size)
    {
        // do something
    }

    void my_free(void* ptr)
    {
        // do something
    }

    #define LIBFS_MALLOC my_malloc
    #define LIBFS_FREE my_free

Custom malloc/free with Hooks
-----------------------------

Alternatively you can use ``fs_init_hooks`` to register custom malloc or free functions
at runtime for custom memory management:

.. code-block:: c

    void* my_malloc(size_t size)
    {
        // do something
    }

    void my_free(void* ptr)
    {
        // do something
    }

    int main(void)
    {
        struct fs_hooks my_hooks = { my_malloc, my_free };
        fs_init_hooks(&my_hooks);

        return 0;
    }
