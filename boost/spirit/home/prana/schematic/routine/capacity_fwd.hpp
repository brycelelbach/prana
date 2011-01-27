/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_0129896A_8627_458D_82F1_5D15A818842D)
#define BSP_0129896A_8627_458D_82F1_5D15A818842D

#include <boost/spirit/home/prana/dispatch/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/adt/tuple.hpp>
#include <boost/spirit/home/prana/support/half_t.hpp>
#include <boost/spirit/home/prana/schematic/exception.hpp>
#include <boost/spirit/home/prana/schematic/cast.hpp>
#include <boost/spirit/home/prana/magic.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class X>
inline uinthalf_t capacity (X const& x);

namespace routine {

template<class TagX, class Dummy = void>
struct capacity;

} // routine

} // prana
} // spirit
} // boost

#endif // BSP_0129896A_8627_458D_82F1_5D15A818842D

