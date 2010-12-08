/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_SET_CAR_FWD_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_SET_CAR_FWD_HPP

#include <boost/spirit/home/prana/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/adt/tuple.hpp>
#include <boost/spirit/home/prana/sexpr/exception.hpp>
#include <boost/spirit/home/prana/sexpr/cast.hpp>
#include <boost/spirit/home/prana/sexpr/core_fwd.hpp>
#include <boost/spirit/home/prana/sexpr/traits.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[set_car_declaration
template<class Value>
inline sexpr*& set_car (sexpr&, Value const&);

inline sexpr*& set_car (sexpr&, sexpr* const&);
//]

namespace routine {

//[routine_set_car_declaration
template<class Tag, class Dummy = prana::unused_type>
struct set_car;
//]

} /*<- routine ->*/

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_SET_CAR_FWD_HPP ->*/

