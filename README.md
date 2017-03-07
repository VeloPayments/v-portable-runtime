Velo Portable Runtime
=====================

The Velo Portable Runtime is a runtime wrapper for C that provides some
abstraction over operating system and data structure features.  This ensures
that code written with the vpr can be ported to many different systems without
making too many assumptions about how the underlying system operates.

Current Features
-----------------

* Disposable interface.
* Portable Memory Allocation Library.
* Dynamic Arrays.
* Abstract Factory.

Building
========

The [`vc-toolchain`][vc-toolchain-url] project needs to be installed in the
`/opt/vctoolchain` subdirectory.  If a different installation directory is used,
then the `TOOLCHAIN_DIR` environment variable should be set to that directory
instead.

[vc-toolchain-url]: https://github.com/VeloPayments/vc-toolchain

The default build target will just build the vpr release library for each
supported platform.  To run unit tests, use the `test` build target.  This will
build both the release and the checked libraries.  The `test` build target
depends on Google Test.  The location of the Google Test source distribution
must be included in the `GTEST_DIR` environment variable.  For instance:

    #build just the release libraries
    make
    
    #build with unit tests - also builds checked libraries
    GTEST_DIR="path/to/google/test" make test

The resulting library will be available under the `build` subdirectory, which
will be created as part of the build process.

This library also supports model checking via [CBMC][cbmc-url].  To run the
model checks, use the following build target.  Note that the `cbmc` executable
must be in the current `PATH`.

    #run model checks
    make model-check

[cbmc-url]: http://www.cprover.org/cbmc/

Continuous Integration Recommendations
--------------------------------------

It is recommended that this project be run as part of the upstream work for any
libraries that depend on it in the build pipeline.  Additionally, all three of
the supported build targets, `make`, `make model-check`, and `make test` should
be run as described in the previous section.  If any of these build targets
fail, then the build should be considered as a failure.
