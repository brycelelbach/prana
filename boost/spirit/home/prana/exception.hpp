/*==============================================================================
    Copyright (c) 2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_EXCEPTION_HPP)
#define BOOST_SPIRIT_PRANA_EXCEPTION_HPP

#include <boost/throw_exception>

#include <boost/spirit/home/prana/source_location.hpp>

namespace boost {
namespace spirit {
namespace prana {
    
struct exception: boost::exception {
  std::string msg;

  exception (void) {
    msg = "(unknown-exception)";
  }
  
  exception (std::string const& source, source_location loc,
             std::string const& exception)
  {
    set(source, loc, exception); 
  }

  void set (std::string const& source, source_location loc,
            std::string const& exception)
  {
    msg = "(exception \"" + source + "\" " + loc.get() + " '" + exception + ")";    
  }

  virtual ~exception (void) throw() { }

  virtual const char* what (void) const throw() {
    return msg.c_str();
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_EXCEPTION_HPP

