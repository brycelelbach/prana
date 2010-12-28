/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_EXCEPTION_HPP)
#define BOOST_SPIRIT_PRANA_VM_EXCEPTION_HPP

#include <sstream>

#include <boost/throw_exception.hpp>

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct vm_exception: std::exception { };

struct compilation_error: vm_exception {
  ~compilation_error (void) throw() { }

  virtual char const* what (void) const throw() {
    return "'(compilation-error)";
  }
};

struct expected_identifier: vm_exception {
  std::string msg;
  
  expected_identifier (void) {
    msg = "'(expected-identifier)";
  }

  expected_identifier (utree const& x) {
    std::ostringstream oss;
    oss << "'(expected-identifier " << x << ")"; 
    msg = oss.str();
  }

  ~expected_identifier (void) throw() { }

  virtual char const* what (void) const throw() {
    return msg.c_str();
  }
};

struct unexpected_identifier: vm_exception {
  std::string msg;
  
  unexpected_identifier (void) {
    msg = "'(unexpected-identifier)";
  }

  unexpected_identifier (utree const& x) {
    std::ostringstream oss;
    oss << "'(unexpected-identifier " << x << ")"; 
    msg = oss.str();
  }

  ~unexpected_identifier (void) throw() { }

  virtual char const* what (void) const throw() {
    return msg.c_str();
  }
};

struct identifier_not_found: vm_exception {
  std::string msg;

  identifier_not_found (std::string const& id) {
    msg = "'(identifier-not-found " + id + ")"; 
  }

  ~identifier_not_found (void) throw() { }

  virtual char const* what (void) const throw() {
    return msg.c_str();
  }
};

struct duplicate_identifier: vm_exception {
  std::string msg;
  
  duplicate_identifier (void) {
    msg = "'(duplicate-identifier)";
  }

  duplicate_identifier (std::string const& id) {
    msg = "'(duplicate-identifier " + id + ")";
  }

  ~duplicate_identifier (void) throw() { }

  virtual char const* what (void) const throw() {
    return msg.c_str();
  }
};

struct body_already_defined: vm_exception {
  std::string msg;

  body_already_defined (std::string const& id) {
    msg = "'(body-already-defined " + id + ")";
  }

  ~body_already_defined (void) throw() { }

  virtual char const* what (void) const throw() {
    return msg.c_str();
  }
};

struct incorrect_arity: vm_exception {
  std::string msg;

  incorrect_arity (std::string const& id, int arity, bool fixed) {
    std::ostringstream oss;
    oss << "'(incorrect-arity " << id << " "
        << arity << " " << (fixed ? "#t" : "#f") << ")"; 
    msg = oss.str();
  }

  ~incorrect_arity (void) throw() { }

  virtual char const* what (void) const throw() {
    return msg.c_str();
  }
};

struct expected_function_application: vm_exception {
  std::string msg;

  expected_function_application (utree const& x) {
    std::ostringstream oss;
    oss << "'(expected-function-application " << x << ")";
    msg = oss.str();
  }

  ~expected_function_application (void) throw() { }

  virtual char const* what (void) const throw() {
    return msg.c_str();
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_EXCEPTION_HPP

