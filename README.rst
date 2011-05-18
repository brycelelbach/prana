Prerequisites
=============

  * Kernel: `Linux <http://git.kernel.org/?p=linux/kernel/git/torvalds/linux-2.6.git;a=summary>`_ (2.6.3*+)
  * C++ compiler: `GCC <http://gcc.gnu.org>`_ (4.4.* or 4.5.*) or `Clang <http://clang.llvm.org>`_ (2.9+) 
  * C++ standard library: `GNU libstdc++ <http://gcc.gnu.org/libstdc++/>`_ (4.4.* or 4.5.*)
  * C standard library: `EGLIBC <http://www.eglibc.org/home>`_ (2.7.*+) or `glibc <http://www.gnu.org/s/libc>`_ (2.7.*+)
  * `Boost C++ Libraries <http://www.boost.org>`_ (1.47.0+)
  * `Boost Build <http://www.boost.org/boost-build2>`_ (v2+)

*Note: Support for Windows (7, HPC Compute Cluster or Server) + MSVC 10 is forthcoming.*

Building
========

  0) Set the environmental variable BOOST_ROOT to point to your Boost source
     tree:::

      $ export BOOST_ROOT=/home/wash/boost

  1) Invoke bjam:::

      $ bjam -j40

License
=======

Prana and Phxpr use the `Boost Software License <http://www.boost.org/LICENSE_1_0.txt>`_.

