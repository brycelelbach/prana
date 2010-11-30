/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_BECOME_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_BECOME_HPP

#include <boost/type_traits/is_same.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/spirit/home/prana/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/sexpr/cast.hpp>
#include <boost/spirit/home/prana/sexpr/traits.hpp>
#include <boost/spirit/home/prana/sexpr/clear.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[become_declaration
template<class To, class X>
inline void become (X& x);
//]

namespace functor {

//[functor_become_declaration
template<class TagX, class Dummy = prana::unused_type>
struct become;
//]

//[functor_become_definition
template<class To, class From>
struct clear {
  struct implementation_functor; 

  typedef void result_type;
  
  template<class X>
  result_type operator() (X& x) const {
    prana::clear(x);
    // TODO (wash): implement 
  }
};

template<class To, class From>
struct clear<
  To, From, typename enable_if<
    is_same<To, From>, 
    prana::unused_type
  >::type
> {
  struct implementation_functor;

  typedef void result_type;

  template<class X>
  result_type operator() (X& x) const { } /*<- If To and From are the same type
                                               definition, do nothing. ->*/
};
//]

} /*<- functor ->*/

//[become_definition
template<class To, class X>
inline void become (X& x) {
  // TODO (wash): implement
  return dispatch<typename X::registry, functor::clear, X>(x);
}
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_BECOME_HPP ->*/

