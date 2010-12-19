/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_EXCEPTION_HPP)
#define BOOST_SPIRIT_PRANA_EXCEPTION_HPP

#include <sstream>

#include <boost/throw_exception.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct dispatch_exception: std::exception { };

struct invalid_type_information: dispatch_exception {
  std::string msg;

  invalid_type_information (std::size_t type) {
    std::ostringstream oss;
    oss << "'(invalid-type-information " << type << ")";
    msg = oss.str();
  }

  virtual ~invalid_type_information (void) throw() { }

  virtual const char* what (void) const throw() {
    return msg.c_str();
  }
};

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_EXCEPTION_HPP ->*/
