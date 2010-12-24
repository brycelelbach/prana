/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SET_CDR_FWD_HPP)
#define BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SET_CDR_FWD_HPP

#include <boost/spirit/home/prana/dispatch/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/adt/tuple.hpp>
#include <boost/spirit/home/prana/schematic/exception.hpp>
#include <boost/spirit/home/prana/schematic/cast.hpp>
#include <boost/spirit/home/prana/schematic/core_fwd.hpp>
#include <boost/spirit/home/prana/schematic/traits.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[set_cdr_declaration
template<class Value>
inline schematic*& set_cdr (schematic&, Value const&);

inline schematic*& set_cdr (schematic&, schematic* const&);
//]

namespace routine {

//[routine_set_cdr_declaration
template<class Tag, class Dummy = void>
struct set_cdr;
//]

} // routine

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SET_CDR_FWD_HPP

