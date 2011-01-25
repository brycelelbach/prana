/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/adt/source_location.hpp> 

namespace {
  void stringify_location (boost::spirit::prana::source_location const& loc,
                           std::string& s)
  {
    s = loc.get();
  }
}

int main (void) { try {
  using boost::spirit::prana::source_location;

  std::cout << "unknown location test" << std::endl; 

  { 
    source_location loc = {-1, -1};

    std::cout << loc << std::endl;

    BSP_BOOLEAN_TESTS(
      ((loc.line()) (-1))
      ((loc.column()) (-1)))

    BSP_STRINGIFY_TESTS(
      stringify_location,
      ((loc) ("(? ?)")))
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}

