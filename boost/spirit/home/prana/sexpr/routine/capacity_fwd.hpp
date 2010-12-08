/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_CAPACITY_FWD_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_CAPACITY_FWD_HPP

#include <boost/spirit/home/prana/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/adt/tuple.hpp>
#include <boost/spirit/home/prana/support/half_t.hpp>
#include <boost/spirit/home/prana/sexpr/exception.hpp>
#include <boost/spirit/home/prana/sexpr/cast.hpp>
#include <boost/spirit/home/prana/sexpr/traits.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[capacity_declaration
template<class X>
inline uinthalf_t capacity (X const& x);
//]

namespace routine {

//[routine_capacity_declaration
template<class TagX, class Dummy = prana::unused_type>
struct capacity;
//]

} /*<- routine ->*/

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_CAPACITY_FWD_HPP ->*/

