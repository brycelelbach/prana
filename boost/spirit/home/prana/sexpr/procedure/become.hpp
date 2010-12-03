/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_PROCEDURE_BECOME_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_PROCEDURE_BECOME_HPP

#include <boost/type_traits/is_same.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/spirit/home/prana/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/sexpr/cast.hpp>
#include <boost/spirit/home/prana/sexpr/traits.hpp>
#include <boost/spirit/home/prana/sexpr/procedure/clear.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[become_declaration
template<class To, class X>
inline void become (X& x);
//]

namespace procedure {

//[procedure_become_declaration
template<class To, class From, class Dummy = prana::unused_type>
struct become;
//]

//[procedure_become_definition
template<class To, class From, class Dummy>
struct become {
  struct procedure; 

  typedef void result_type;
  
  template<class X>
  result_type operator() (X& x) const {
    prana::clear(x);
    x.type = To::value; 
  }
};

template<class To, class From>
struct become<
  To, From, typename enable_if<
    is_same<To, From>, 
    prana::unused_type
  >::type
> {
  struct procedure;

  typedef void result_type;

  template<class X>
  result_type operator() (X& x) const { } /*<- If To and From are the same type
                                               definition, do nothing. ->*/
};
//]

} /*<- procedure ->*/

//[become_definition
template<class To, class X>
inline void become (X& x) {
  return dispatch<typename X::registry, procedure::become, To, X>(x);
}
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_PROCEDURE_BECOME_HPP ->*/

