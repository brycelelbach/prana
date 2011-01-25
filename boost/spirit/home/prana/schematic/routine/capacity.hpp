/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_SCHEMATIC_ROUTINE_CAPACITY_HPP)
#define BSP_SCHEMATIC_ROUTINE_CAPACITY_HPP

#include <boost/spirit/home/prana/schematic/routine/capacity_fwd.hpp>

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
    traits::is_dynamic_array_type<TagX>
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

} // routine

//[capacity_definition
template<class X>
inline uinthalf_t capacity (X const& x) {
  return dispatch<typename X::registry, routine::capacity, X const>(x);
}
//]

} // prana
} // spirit
} // boost

#endif // BSP_SCHEMATIC_ROUTINE_CAPACITY_HPP

