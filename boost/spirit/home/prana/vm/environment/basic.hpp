/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_VM_ENVIRONMENT_BASIC_HPP)
#define BSP_VM_ENVIRONMENT_BASIC_HPP

#include <string>

#include <boost/unordered_map.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Derived, class Value, class Result = Value,
         class Key = std::string, class Hash = boost::hash<Key>,
         class Pred = std::equal_to<Key> >
struct basic_environment {
  typedef basic_environment<Derived, Value, Result, Key, Hash, Pred> base_type;
  
  typedef Value value_type;
  
  typedef Result result_type;

  typedef Key key_type;

  typedef boost::unordered_map<key_type, value_type, Hash, Pred> map_type;

  typedef typename map_type::iterator iterator;
  typedef typename map_type::const_iterator const_iterator;

  basic_environment (Derived* parent = 0, Hash const& hash = Hash(),
                     Pred const& pred = Pred()):
    outer(parent), depth(parent ? parent->depth + 1 : 0),
    definitions(boost::unordered_detail::default_bucket_count, hash, pred) { }
  
  Derived& derived (void) {
    return *static_cast<Derived*>(this);
  }
  
  Derived const& derived (void) const {
    return *static_cast<Derived const*>(this);
  }

  result_type operator() (key_type const& name) {
    return derived().find(name);
  }

  void undefine (key_type const& name) {
    definitions.erase(name);
  }

  bool defined (key_type const& name) {
    return (*this)(name) != derived().sentinel();
  }

  Derived* parent (void) const {
    return outer;
  }

  unsigned level (void) const {
    return depth;
  }

  Derived* outer;
  unsigned depth;
  map_type definitions;
};

} // prana
} // spirit
} // boost

#endif // BSP_VM_ENVIRONMENT_BASIC_HPP

