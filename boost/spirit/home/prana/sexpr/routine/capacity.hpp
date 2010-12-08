/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_CAPACITY_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_CAPACITY_HPP

#include <boost/spirit/home/prana/sexpr/routine/capacity_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

namespace routine {

//[routine_capacity_definition
template<class TagX, class Dummy>
struct capacity {
  struct routine; 

  typedef uinthalf_t result_type;
  
  template<class X>
  result_type operator() (X& x) const {
    BOOST_THROW_EXCEPTION(expected_dynamic_array(x));
  }
};
//]

//[routine_capacity_dynamic_array_specialization
template<class TagX>
struct capacity<
  TagX, typename enable_if<
    is_dynamic_array_type<TagX>,
    prana::unused_type
  >::type
> {
  struct routine; 

  typedef uinthalf_t result_type;
  
  template<class X>
  result_type operator() (X& x) const {
    return value_at<1>(*cast<TagX>(x)); 
  }
};
//]

} /*<- routine ->*/

//[capacity_definition
template<class X>
inline uinthalf_t capacity (X const& x) {
  return dispatch<typename X::registry, routine::capacity, X const>(x);
}
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_CAPACITY_HPP ->*/

