/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_PROCEDURE_SIZE_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_PROCEDURE_SIZE_HPP

#include <boost/spirit/home/prana/sexpr/routine/size_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

namespace routine {

//[routine_size_definition
template<class TagX, class Dummy>
struct size {
  struct routine; 

  typedef uinthalf_t result_type;
  
  template<class X>
  result_type operator() (X const& x) const {
    BOOST_THROW_EXCEPTION(expected_composite(x));
  }
};
//]

//[routine_size_dynamic_array_specialization
template<class TagX>
struct size<
  TagX, typename enable_if<
    is_dynamic_array_type<TagX>,
    prana::unused_type
  >::type
> {
  struct routine; 

  typedef uinthalf_t result_type;
  
  template<class X>
  result_type operator() (X const& x) const {
    return value_at<0>(*cast<TagX>(x)); 
  }
};
//]

} /*<- routine ->*/

//[size_definition
template<class X>
inline uinthalf_t size (X const& x) {
  return dispatch<typename X::registry, routine::size, X const>(x);
}
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_PROCEDURE_SIZE_HPP ->*/

