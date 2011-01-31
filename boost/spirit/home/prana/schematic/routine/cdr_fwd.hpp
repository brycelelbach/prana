/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_SCHEMATIC_ROUTINE_CDR_FWD_HPP)
#define BSP_SCHEMATIC_ROUTINE_CDR_FWD_HPP

#include <boost/spirit/home/prana/dispatch/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/magic.hpp>
#include <boost/spirit/home/prana/adt/tuple.hpp>
#include <boost/spirit/home/prana/exception.hpp>
#include <boost/spirit/home/prana/schematic/cast.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[cdr_declaration
template<class X>
inline X* cdr (X& x);

template<class X>
inline X const* cdr (X const& x);
//]

namespace routine {

//[routine_cdr_declaration
template<class TagX, class Dummy = void>
struct cdr;
//]

} // routine

} // prana
} // spirit
} // boost

#endif // BSP_SCHEMATIC_ROUTINE_CDR_FWD_HPP

