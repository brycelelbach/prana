/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_CLEAR_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_CLEAR_HPP

#include <boost/mpl/or.hpp>

#include <boost/utility/result_of.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/spirit/home/prana/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/sexpr/cast.hpp>
#include <boost/spirit/home/prana/sexpr/traits.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[sexpr_clear_interface_declaration
template<class X>
inline void clear (X& x);
//]

namespace functor {

//[sexpr_clear_implementation_declaration
template<class TagX, class Dummy = prana::unused_type>
struct clear;
//]

//[sexpr_clear_implementation_definition
template<class TagX, class Dummy>
struct clear {
  struct implementation_functor; /*<- This incomplete type is a has_xxx
                                      detection member. ->*/

  typedef void result_type; /*<- The result_type typedef provides result_of
                                 support. ->*/
  
  template<class X>
  result_type operator() (X& x) const {
    x.type = X::nil::value;

    x.car = 0;

    if (x.cdr) {
      prana::clear(*x.cdr); /*<- If the cdr of the s-expression is not nil,
                                 call the interface function. ->*/
      delete x.cdr;
    }
  }
};
//]

//[sexpr_clear_implementation_specialization
template<class TagX>
struct clear<
  TagX, typename enable_if<
    is_heap_allocated_type<TagX>, /*<- is_heap_allocated_type is an MPL
                                       metafunction predicate that returns true
                                       if it's subject is a member of a
                                       type definition category which is
                                       allocated on the heap. ->*/
    prana::unused_type
  >::type
> {
  struct implementation_functor;

  typedef void result_type;

  template<class X>
  void operator() (X& x) const {
    x.type = X::nil::value;

    typename TagX::data_type* p = cast_car<TagX>(x); /*<- cast_car transforms
                                                          the car member of the
                                                          supplied sexpr into
                                                          the data_type of the
                                                          provided type
                                                          definition. ->*/
    if (p && *p) {
      delete *p;
      x.car = 0;
    }

    if (x.cdr) {
      prana::clear(*x.cdr); 
      delete x.cdr;
      x.cdr = 0;
    }
  }
};
//]

} /*<- functor ->*/

//[sexpr_clear_interface_definition
template<class X>
inline void clear (X& x) {
  return dispatch<typename X::registry, functor::clear, X>(x);
}
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_CLEAR_HPP ->*/

