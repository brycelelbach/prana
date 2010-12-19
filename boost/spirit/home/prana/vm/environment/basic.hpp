/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_BASIC_HPP)
#define BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_BASIC_HPP

#include <string>
#include <map>

namespace boost {
namespace spirit {
namespace prana {

template<class Derived, class Value, class Result = Value,
         class Key = std::string, class Compare = std::less<Key> >
struct basic_environment {
  typedef basic_environment<Derived, Value, Result, Key> base_type;
  
  typedef Value value_type;
  
  typedef Result result_type;

  typedef Key key_type;

  typedef typename std::map<key_type, value_type, Compare>::iterator iterator;

  basic_environment (Derived* parent = 0, Compare const& comp = Compare()):
    outer(parent), depth(parent ? parent->depth + 1 : 0), definitions(comp) { }
  
  Derived const& derived (void) const {
    return *static_cast<Derived const*>(this);
  }

  result_type operator() (key_type const& name) {
    return derived().find(name);
  }

  void undefine (key_type const& name) {
    definitions.erase(name);
  }

  bool defined (key_type const& name) const {
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
  std::map<key_type, value_type, Compare> definitions;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_BASIC_HPP

