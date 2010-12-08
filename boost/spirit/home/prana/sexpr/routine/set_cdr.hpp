/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_SET_CDR_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_SET_CDR_HPP

#include <boost/spirit/home/prana/sexpr/routine/set_cdr_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

namespace routine {

//[routine_set_cdr_definition
template<class Tag, class Dummy>
struct set_cdr {
  struct routine; 

  typedef sexpr*& result_type;
  
  template<class Value>
  result_type operator() (sexpr& s, Value const&) const {
    BOOST_THROW_EXCEPTION(expected_cons(s));
  }

  result_type operator() (sexpr& s, sexpr* const&) const {
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
    >,
    prana::unused_type
  >::type
> {
  struct routine; 

  typedef sexpr*& result_type;
  
  result_type operator() (sexpr& s, spirit::unused_type const&) const {
    sexpr*& p = value_at<1>(*cast<Tag>(s));

    if (p)
      delete p;
    p = 0; 

    return p;
  }
 
  template<class Value>
  result_type operator() (sexpr& s, Value const& v) const {
    sexpr*& p = value_at<1>(*cast<Tag>(s));

    if (p)
      p->assign(v);
    else
      p = new sexpr(v);

    return p;
  }
  
  result_type operator() (sexpr& s, sexpr* const& v) const {
    sexpr*& p = value_at<1>(*cast<Tag>(s));
    
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
    >,
    prana::unused_type
  >::type
> {
  struct routine; 

  typedef sexpr*& result_type;
  
  result_type operator() (sexpr& s, spirit::unused_type const&) const {
    sexpr*& p = value_at<1>(*cast<Tag>(s));
    p = 0; 
    return p;
  }
 
  template<class Value>
  result_type operator() (sexpr& s, Value const&) const {
    BOOST_THROW_EXCEPTION(storage_unavailable(s));
  }
  
  result_type operator() (sexpr& s, sexpr* const& v) const {
    sexpr*& p = value_at<1>(*cast<Tag>(s));
    p = v;
    return p;
  }
};
//]

} /*<- routine ->*/

//[set_cdr_definition
template<class Value>
inline sexpr*& set_cdr (sexpr& s, Value const& v) {
  return dispatch<sexpr::registry, routine::set_cdr, sexpr, Value const>(s, v);
}

inline sexpr*& set_cdr (sexpr& s, sexpr* const& v) {
  return dispatch<sexpr::registry, routine::set_cdr, sexpr, sexpr* const>(s, v);
}
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_SET_CDR_HPP ->*/

