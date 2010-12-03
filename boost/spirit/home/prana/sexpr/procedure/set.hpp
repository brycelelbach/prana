/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_PROCEDURE_SET_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_PROCEDURE_SET_HPP

#include <boost/spirit/home/prana/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/sexpr/traits.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[set_declaration
template<class X, class Y>
inline void set (X& x, Y const& y);
//]

namespace procedure {

//[procedure_set_declaration
template<class TagX, class Dummy = prana::unused_type>
struct set;
//]

//[procedure_set_definition
template<class TagX, class Dummy>
struct set {
  struct procedure; 

  typedef void result_type;
  
  template<class X, class Y>
  result_type operator() (X& x, Y const& y) const {
    // TODO (wash): implement
  }
};
//]

} /*<- procedure ->*/

//[set_definition
template<class X, class Y>
inline void set (X& x, Y const& y) {
  return dispatch<typename X::registry, procedure::set, X>(x, y);
}
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_PROCEDURE_SET_HPP ->*/

