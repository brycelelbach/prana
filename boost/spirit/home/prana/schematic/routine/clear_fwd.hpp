/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_CLEAR_FWD_HPP)
#define BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_CLEAR_FWD_HPP

#include <boost/utility/enable_if.hpp>

#include <boost/spirit/home/prana/dispatch/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/adt/tuple.hpp>
#include <boost/spirit/home/prana/schematic/cast.hpp>
#include <boost/spirit/home/prana/schematic/traits.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[clear_declaration
template<class X>
inline void clear (X& x);
//]

namespace routine {

//[routine_clear_declaration
template<class TagX, class Dummy = void>
struct clear;
//]

} // routine

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_CLEAR_FWD_HPP

