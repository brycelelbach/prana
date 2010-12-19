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

template<class Derived, class Value, class Result = Value>
struct basic_environment {
  typedef basic_environment<Derived, Value, Result> base_type;
  
  typedef Value value_type;
  
  typedef Result result_type;

  basic_environment (Derived* parent = 0):
    outer(parent), depth(parent ? parent->depth + 1 : 0) { }
  
  Derived const& derived (void) const {
    return *static_cast<Derived const*>(this);
  }

  result_type operator() (std::string const& name) {
    return derived().find(name);
  }

  void undefine (std::string const& name) {
    definitions.erase(name);
  }

  bool defined (std::string const& name) const {
    return definitions.find(name) != definitions.end();
  }

  Derived* parent (void) const {
    return outer;
  }

  unsigned level (void) const {
    return depth;
  }

  Derived* outer;
  unsigned depth;
  std::map<std::string, value_type> definitions;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_BASIC_HPP

