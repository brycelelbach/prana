/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_SCHEMATIC_ROUTINE_SET_CAR_FWD_HPP)
#define BSP_SCHEMATIC_ROUTINE_SET_CAR_FWD_HPP

#include <boost/spirit/home/prana/dispatch/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/adt/tuple.hpp>
#include <boost/spirit/home/prana/exception.hpp>
#include <boost/spirit/home/prana/schematic/cast.hpp>
#include <boost/spirit/home/prana/schematic/core_fwd.hpp>
#include <boost/spirit/home/prana/magic.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[set_car_declaration
template<class Value>
inline schematic*& set_car (schematic&, Value const&);

inline schematic*& set_car (schematic&, schematic* const&);
//]

namespace routine {

//[routine_set_car_declaration
template<class Tag, class Dummy = void>
struct set_car;
//]

} // routine

} // prana
} // spirit
} // boost

#endif // BSP_SCHEMATIC_ROUTINE_SET_CAR_FWD_HPP

