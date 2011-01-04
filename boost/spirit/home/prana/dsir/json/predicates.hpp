/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_DSIR_JSON_PREDICATES_HPP)
#define BOOST_SPIRIT_PRANA_DSIR_JSON_PREDICATES_HPP

#include <boost/spirit/include/support_utree.hpp>
#include <boost/foreach.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct is_key_predicate { 
  typedef bool result_type;

  result_type operator() (utree const& ut) const {
    return utree::visit(ut, *this);
  }

  result_type operator() (utf8_symbol_range_type const& str) const {
    return true; 
  }
  
  template<typename T>
  result_type operator() (T const& t) const {
    return false;
  }
};

template<typename T>
bool is_key (T const& t) {
  return is_key_predicate()(t);
}

struct is_member_pair_predicate { 
  typedef bool result_type;

  result_type operator() (utree const& ut) const {
    return utree::visit(ut, *this);
  }

  template<typename Iterator>
  result_type operator() (iterator_range<Iterator> const& range) const {
    if (range.size() != 2)
      return false;
    
    return is_key(*range.begin());
  }
  
  template<typename T>
  result_type operator() (T const& t) const {
    return false;
  }
};

template<typename T>
bool is_member_pair (T const& t) {
  return is_member_pair_predicate()(t);
}

struct is_object_predicate {
  typedef bool result_type;
 
  result_type operator() (utree const& ut) const {
    return utree::visit(ut, *this);
  }
 
  template<typename Iterator>
  result_type operator() (iterator_range<Iterator> const& range) const {
    BOOST_FOREACH(utree const& it, range) { 
      if (!is_member_pair(it))
        return false; 
    }

    return true;
  }

  template<typename T>
  result_type operator() (T const& t) const {
    return false;
  }
};

template<typename T>
bool is_object (T const& t) {
  return is_object_predicate()(t);
}

struct is_array_predicate {
  typedef bool result_type;
 
  result_type operator() (utree const& ut) const {
    return utree::visit(ut, *this);
  }
 
  template<typename Iterator>
  result_type operator() (iterator_range<Iterator> const& range) const {
    return !is_object(range);
  }

  template<typename T>
  result_type operator() (T const& t) const {
    return false;
  }
};

template<typename T>
bool is_array (T const& t) {
  return is_array_predicate()(t);
}

struct is_list_predicate {
  typedef bool result_type;
 
  result_type operator() (utree const& ut) const {
    return utree::visit(ut, *this);
  }
 
  template<typename Iterator>
  result_type operator() (iterator_range<Iterator> const& range) const {
    return true; 
  }

  template<typename T>
  result_type operator() (T const& t) const {
    return false;
  }
};

template<typename T>
bool is_list (T const& t) {
  return is_list_predicate()(t);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_DSIR_JSON_PREDICATES_HPP

