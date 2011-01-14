/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_CAR_HPP)
#define BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_CAR_HPP

#include <boost/spirit/home/prana/schematic/routine/car_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

namespace routine {

//[routine_car_definition
template<class TagX, class Dummy>
struct car {
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

//[routine_car_cons_specialization
template<class TagX>
struct car<
  TagX, typename enable_if<
    traits::is_cons_type<TagX>
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
    return value_at<0>(*cast<TagX>(x)); 
  }
};
//]

} // routine

//[car_definition
template<class X>
inline X* car (X& x) {
  return dispatch<typename X::registry, routine::car, X>(x);
}

template<class X>
inline X const* car (X const& x) {
  return dispatch<typename X::registry, routine::car, X const>(x);
}
//]

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_CAR_HPP

