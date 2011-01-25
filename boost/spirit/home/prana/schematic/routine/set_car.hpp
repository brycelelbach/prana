/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_SCHEMATIC_ROUTINE_SET_CAR_HPP)
#define BSP_SCHEMATIC_ROUTINE_SET_CAR_HPP

#include <boost/spirit/home/prana/schematic/routine/set_car_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

namespace routine {

//[routine_set_car_definition
template<class Tag, class Dummy>
struct set_car {
  struct routine; 

  typedef schematic*& result_type;
  
  template<class Value>
  result_type operator() (schematic& s, Value const&) const {
    BOOST_THROW_EXCEPTION(expected_cons(s));
  }

  result_type operator() (schematic& s, schematic* const&) const {
    BOOST_THROW_EXCEPTION(expected_cons(s));
  } 
};
//]

//[routine_set_car_cons_heap_specialization
template<class Tag>
struct set_car<
  Tag, typename enable_if<
    mpl::and_<
      traits::is_heap_allocated_type<Tag>,
      traits::is_cons_type<Tag>
    >
  >::type
> {
  struct routine; 

  typedef schematic*& result_type;
  
  result_type operator() (schematic& s, spirit::unused_type const&) const {
    schematic*& p = value_at<0>(*cast<Tag>(s));

    if (p)
      delete p;
    p = 0; 

    return p;
  }
 
  template<class Value>
  result_type operator() (schematic& s, Value const& v) const {
    schematic*& p = value_at<0>(*cast<Tag>(s));

    if (p)
      p->assign(v);
    else
      p = new schematic(v);

    return p;
  }
  
  result_type operator() (schematic& s, schematic* const& v) const {
    schematic*& p = value_at<0>(*cast<Tag>(s));
    
    if (p)
      delete p;
    p = v;
    
    return p;
  }
};

//[routine_set_car_cons_stack_specialization
template<class Tag>
struct set_car<
  Tag, typename enable_if<
    mpl::and_<
      traits::is_stack_allocated_type<Tag>,
      traits::is_cons_type<Tag>
    >
  >::type
> {
  struct routine; 

  typedef schematic*& result_type;
  
  result_type operator() (schematic& s, spirit::unused_type const&) const {
    schematic*& p = value_at<0>(*cast<Tag>(s));
    p = 0; 
    return p;
  }
 
  template<class Value>
  result_type operator() (schematic& s, Value const&) const {
    BOOST_THROW_EXCEPTION(storage_unavailable(s));
  }
  
  result_type operator() (schematic& s, schematic* const& v) const {
    schematic*& p = value_at<0>(*cast<Tag>(s));
    p = v;
    return p;
  }
};
//]

} // routine

//[set_car_definition
template<class Value>
inline schematic*& set_car (schematic& s, Value const& v) {
  return dispatch<
    schematic::registry, routine::set_car, schematic, Value const
  >(s, v);
}

inline schematic*& set_car (schematic& s, schematic* const& v) {
  return dispatch<
    schematic::registry, routine::set_car, schematic, schematic* const
  >(s, v);
}
//]

} // prana
} // spirit
} // boost

#endif // BSP_SCHEMATIC_ROUTINE_SET_CAR_HPP

