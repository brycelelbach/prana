Prerequisites
=============

  * Kernel: Linux
  * C++ compiler: `GCC <http://gcc.gnu.org>` (4.4.* or 4.5.*) or `Clang <http://clang.llvm.org>` (2.9+) 
  * C++ standard library: `GNU libstdc++ <http://gcc.gnu.org/libstdc++/>` (4.4.* or 4.5.*)
  * C standard library: `EGLIBC <http://www.eglibc.org/home>` (2.7.*+) or `glibc <http://www.gnu.org/s/libc>` (2.7.*+)
  * `Boost C++ Libraries <http://www.boost.org>` (1.47.0+)
  * `Boost Build <http://www.boost.org/boost-build2>` (v2+)

Building
========

  0) Set the environmental variable BOOST_ROOT to point to your Boost source
     tree:::

      $ export BOOST_ROOT=/home/wash/boost

  1) Invoke bjam:::

      $ bjam -j40

License
=======

Prana uses the `Boost Software License <http://www.boost.org/LICENSE_1_0.txt>`_.

