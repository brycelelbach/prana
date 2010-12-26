/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SET_CDR_HPP)
#define BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SET_CDR_HPP

#include <boost/spirit/home/prana/schematic/routine/set_cdr_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

namespace routine {

//[routine_set_cdr_definition
template<class Tag, class Dummy>
struct set_cdr {
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

//[routine_set_cdr_cons_heap_specialization
template<class Tag>
struct set_cdr<
  Tag, typename enable_if<
    mpl::and_<
      is_heap_allocated_type<Tag>,
      is_cons_type<Tag>
    >
  >::type
> {
  struct routine; 

  typedef schematic*& result_type;
  
  result_type operator() (schematic& s, spirit::unused_type const&) const {
    schematic*& p = value_at<1>(*cast<Tag>(s));

    if (p)
      delete p;
    p = 0; 

    return p;
  }
 
  template<class Value>
  result_type operator() (schematic& s, Value const& v) const {
    schematic*& p = value_at<1>(*cast<Tag>(s));

    if (p)
      p->assign(v);
    else
      p = new schematic(v);

    return p;
  }
  
  result_type operator() (schematic& s, schematic* const& v) const {
    schematic*& p = value_at<1>(*cast<Tag>(s));
    
    if (p)
      delete p;
    p = v;
    
    return p;
  }
};

//[routine_set_cdr_cons_stack_specialization
template<class Tag>
struct set_cdr<
  Tag, typename enable_if<
    mpl::and_<
      is_stack_allocated_type<Tag>,
      is_cons_type<Tag>
    >
  >::type
> {
  struct routine; 

  typedef schematic*& result_type;
  
  result_type operator() (schematic& s, spirit::unused_type const&) const {
    schematic*& p = value_at<1>(*cast<Tag>(s));
    p = 0; 
    return p;
  }
 
  template<class Value>
  result_type operator() (schematic& s, Value const&) const {
    BOOST_THROW_EXCEPTION(storage_unavailable(s));
  }
  
  result_type operator() (schematic& s, schematic* const& v) const {
    schematic*& p = value_at<1>(*cast<Tag>(s));
    p = v;
    return p;
  }
};
//]

} // routine

//[set_cdr_definition
template<class Value>
inline schematic*& set_cdr (schematic& s, Value const& v) {
  return dispatch<
    schematic::registry, routine::set_cdr, schematic, Value const
  >(s, v);
}

inline schematic*& set_cdr (schematic& s, schematic* const& v) {
  return dispatch<
    schematic::registry, routine::set_cdr, schematic, schematic* const
  >(s, v);
}
//]

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SET_CDR_HPP

