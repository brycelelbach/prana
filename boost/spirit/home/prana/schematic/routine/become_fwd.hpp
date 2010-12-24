/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_BECOME_FWD_HPP)
#define BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_BECOME_FWD_HPP

#include <boost/type_traits/is_same.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/spirit/home/prana/dispatch/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/adt/tuple.hpp>
#include <boost/spirit/home/prana/schematic/cast.hpp>
#include <boost/spirit/home/prana/schematic/traits.hpp>
#include <boost/spirit/home/prana/schematic/routine/clear.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[become_declaration
template<class To, class X>
inline void become (X& x);
//]

namespace routine {

//[routine_become_declaration
template<class To, class From, class Dummy = void>
struct become;
//]

} // routine

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_BECOME_FWD_HPP

