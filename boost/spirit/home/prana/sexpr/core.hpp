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
#include <boost/spirit/home/prana/sexpr/routine/set.hpp>

namespace boost {
namespace spirit {
namespace prana {

sexpr::sexpr (void): type(nil::value) {
  data[0] = 0;
  data[1] = 0;
}

template<class X>
sexpr::sexpr (X x): type(nil::value) {
  data[0] = 0;
  data[1] = 0;
  set(*this, x);
}

sexpr::~sexpr (void) {
  clear(*this);
}

template<class X>
sexpr& sexpr::operator= (X x) {
  // TODO: Add eqv test here.
  set(*this, x);
}

template<class X>
void sexpr::assign (X x) {
  // TODO: Add eqv test here.
  set(*this, x);
}

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_CORE_HPP ->*/

