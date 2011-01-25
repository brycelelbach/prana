/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_SCHEMATIC_ROUTINE_CAPACITY_FWD_HPP)
#define BSP_SCHEMATIC_ROUTINE_CAPACITY_FWD_HPP

#include <boost/spirit/home/prana/dispatch/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/adt/tuple.hpp>
#include <boost/spirit/home/prana/support/half_t.hpp>
#include <boost/spirit/home/prana/schematic/exception.hpp>
#include <boost/spirit/home/prana/schematic/cast.hpp>
#include <boost/spirit/home/prana/traits_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[capacity_declaration
template<class X>
inline uinthalf_t capacity (X const& x);
//]

namespace routine {

//[routine_capacity_declaration
template<class TagX, class Dummy = void>
struct capacity;
//]

} // routine

} // prana
} // spirit
} // boost

#endif // BSP_SCHEMATIC_ROUTINE_CAPACITY_FWD_HPP

