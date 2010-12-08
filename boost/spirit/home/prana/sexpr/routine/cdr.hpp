/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_CDR_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_CDR_HPP

#include <boost/spirit/home/prana/sexpr/routine/cdr_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

namespace routine {

//[routine_cdr_definition
template<class TagX, class Dummy>
struct cdr {
  struct routine; 

  template<class> struct result;
 
  template<class This, class X>
  struct result<This(X&)> {
    typedef X* type;
  };
  
  template<class X>
  X* operator() (X& x) const {
    BOOST_THROW_EXCEPTION(expected_cons(x));
  }
};
//]

//[routine_cdr_cons_specialization
template<class TagX>
struct cdr<
  TagX, typename enable_if<
    is_cons_type<TagX>,
    prana::unused_type
  >::type
> {
  struct routine; 

  template<class> struct result;
 
  template<class This, class X>
  struct result<This(X&)> {
    typedef X* type;
  };
 
  template<class X>
  X* operator() (X& x) const {
    return value_at<1>(*cast<TagX>(x)); 
  }
};
//]

} /*<- routine ->*/

//[cdr_definition
template<class X>
inline X* cdr (X& x) {
  return dispatch<typename X::registry, routine::cdr, X>(x);
}

template<class X>
inline X const* cdr (X const& x) {
  return dispatch<typename X::registry, routine::cdr, X const>(x);
}
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_CDR_HPP ->*/

