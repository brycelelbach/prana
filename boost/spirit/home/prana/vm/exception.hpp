/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_EXCEPTION_HPP)
#define BOOST_SPIRIT_PRANA_VM_EXCEPTION_HPP

#include <sstream>
#include <exception>

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct vm_exception: std::exception { };

struct compilation_error: std::exception {
  ~compilation_error (void) throw() { }

  virtual char const* what (void) const throw() {
    return "compilation error";
  }
};

struct identifier_expected: vm_exception {
  std::string msg;

  identifier_expected (utree const& got) {
    std::ostringstream oss;
    oss << "identifier expected " << got; 
    msg = oss.str();
  }

  ~identifier_expected (void) throw() { }

  virtual char const* what (void) const throw() {
    return msg.c_str();
  }
};

struct identifier_not_found: vm_exception {
  std::string msg;

  identifier_not_found (std::string const& id) {
    std::ostringstream oss;
    oss << "identifier " << id << " not found"; 
    msg = oss.str();
  }

  ~identifier_not_found (void) throw() { }

  virtual char const* what (void) const throw() {
    return msg.c_str();
  }
};

struct duplicate_identifier: vm_exception {
  std::string msg;

  duplicate_identifier (std::string const& id) {
    std::ostringstream oss;
    oss << "duplicate identifier " << id;
    msg = oss.str();
  }

  ~duplicate_identifier (void) throw() { }

  virtual char const* what (void) const throw() {
    return msg.c_str();
  }
};

struct body_already_defined: vm_exception {
  std::string msg;

  body_already_defined (std::string const& id) {
    std::ostringstream oss;
    oss << "multiple definition of " << id;
    msg = oss.str();
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
    oss << "invalid number of parameters to function call "
        << id << ", " << (fixed ? "expecting " : "expecting at least ")
        << arity << " arguments";
    msg = oss.str();
  }

  ~incorrect_arity (void) throw() { }

  virtual char const* what (void) const throw() {
    return msg.c_str();
  }
};

struct function_application_expected: vm_exception {
  std::string msg;

  function_application_expected (utree const& got) {
    std::ostringstream oss;
    oss << "function application expected, got " << got;
    msg = oss.str();
  }

  ~function_application_expected (void) throw() { }

  virtual char const* what (void) const throw() {
    return msg.c_str();
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_EXCEPTION_HPP

