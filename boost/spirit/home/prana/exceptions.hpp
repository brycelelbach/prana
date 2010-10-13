/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_EXCEPTIONS_HPP)
#define BOOST_SPIRIT_PRANA_EXCEPTIONS_HPP

namespace boost {
namespace spirit {
namespace prana {

struct utree_exception: std::exception { };

struct illegal_arithmetic_operation: utree_exception {
  virtual const char* what (void) const throw() {
    return "utree: illegal arithmetic operation.";
  }
};

struct illegal_integral_operation: utree_exception {
  virtual const char* what (void) const throw() {
    return "utree: illegal integral operation.";
  }
};

struct bad_cast: utree_exception {
  virtual const char* what (void) const throw() {
    return "utree: bad cast"; 
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_EXCEPTIONS_HPP
