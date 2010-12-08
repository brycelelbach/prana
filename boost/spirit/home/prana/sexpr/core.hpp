/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_CORE_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_CORE_HPP

#include <boost/spirit/home/prana/sexpr/core_fwd.hpp>
#include <boost/spirit/home/prana/sexpr/routine/clear.hpp>
#include <boost/spirit/home/prana/sexpr/routine/become.hpp>
#include <boost/spirit/home/prana/sexpr/routine/set.hpp>
#include <boost/spirit/home/prana/sexpr/routine/set_car.hpp>
#include <boost/spirit/home/prana/sexpr/routine/set_cdr.hpp>

namespace boost {
namespace spirit {
namespace prana {

sexpr::sexpr (void): type(nil::value) {
  data[0] = 0;
  data[1] = 0;
}

sexpr::~sexpr (void) {
  clear(*this);
}

template<class Value>
sexpr::sexpr (Value const& val): type(nil::value) {
  data[0] = 0;
  data[1] = 0;
  set(*this, val);
}

template<class Value>
sexpr& sexpr::operator= (Value const& val) {
  // TODO: Add eqv test here.
  set(*this, val);
}

template<class Value>
void sexpr::assign (Value const& val) {
  // TODO: Add eqv test here.
  set(*this, val);
}

sexpr::sexpr (tuple2<sexpr*, sexpr*> const& range): type(nil::value) {
  data[0] = 0;
  data[1] = 0;
  assign(range);
}

void sexpr::assign (tuple2<sexpr*, sexpr*> const& range) {
  become<list>(*this);
  set_car(*this, value_at<0>(range));
  set_cdr(*this, value_at<1>(range)); 
}

template<class Car, class Cdr>
sexpr::sexpr (Car car, Cdr cdr): type(nil::value) {
  data[0] = 0;
  data[1] = 0;
  assign(car, cdr);
}

template<class Car, class Cdr>
void sexpr::assign (Car car, Cdr cdr) {
  become<pair>(*this);
  set_car(*this, car);
  set_cdr(*this, cdr); 
}

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_CORE_HPP ->*/

