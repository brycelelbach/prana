/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SIZE_HPP)
#define BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SIZE_HPP

#include <boost/spirit/home/prana/schematic/routine/size_fwd.hpp>

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
  result_type operator() (X& x) const {
    BOOST_THROW_EXCEPTION(expected_dynamic_array(x));
  }
};
//]

//[routine_size_dynamic_array_specialization
template<class TagX>
struct size<
  TagX, typename enable_if<
    traits::is_dynamic_array_type<TagX>
  >::type
> {
  struct routine; 

  typedef uinthalf_t result_type;
  
  template<class X>
  result_type operator() (X& x) const {
    return value_at<0>(*cast<TagX>(x)); 
  }
};
//]

} // routine

//[size_definition
template<class X>
inline uinthalf_t size (X const& x) {
  return dispatch<typename X::registry, routine::size, X const>(x);
}
//]

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SIZE_HPP

