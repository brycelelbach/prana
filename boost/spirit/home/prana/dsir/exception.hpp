/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_DSIR_EXCEPTION_HPP)
#define BSP_DSIR_EXCEPTION_HPP

#include <sstream>

#include <boost/throw_exception.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct dsir_exception: std::exception { };

struct expected_variant_type: dsir_exception {
  std::string msg;

  template<class T>
  expected_variant_type (T type) {
    std::ostringstream oss;
    oss << "'(expected-variant-type " << type << ")";
    msg = oss.str();
  }

  virtual ~expected_variant_type (void) throw() { }

  virtual const char* what (void) const throw() {
    return msg.c_str();
  }
};

struct expected_valid_pointer: dsir_exception {
  virtual ~expected_valid_pointer (void) throw() { }

  virtual const char* what (void) const throw() {
    return "'(expected-valid-pointer)";
  }
};

} // prana
} // spirit
} // boost

#endif // BSP_DSIR_EXCEPTION_HPP

