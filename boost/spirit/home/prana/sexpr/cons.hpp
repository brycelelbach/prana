/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_CONS_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_CONS_HPP

#include <boost/spirit/home/prana/sexpr/core_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[cons_definition
template<class Car>
inline tuple2<sexpr*, sexpr*> cons (Car car,
                                    tuple2<sexpr*, sexpr*> const& cdr) {
  sexpr* p = new sexpr(car, value_at<0>(cdr));
  return make_tuple2<sexpr*, sexpr*>(p, value_at<1>(cdr));
}

template<class Car, class Cdr>
inline tuple2<sexpr*, sexpr*> cons (Car car, Cdr cdr) {
  sexpr* p = new sexpr(car, cdr);
  return make_tuple2<sexpr*, sexpr*>(p, p);
}
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_CONS_HPP ->*/

