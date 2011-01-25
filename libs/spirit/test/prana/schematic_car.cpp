/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>
#include <boost/spirit/home/prana/schematic/routine/car.hpp>

int main (void) { try {
  using boost::spirit::prana::schematic;
  using boost::spirit::prana::car;

  { //[integer_integer_pair
    schematic s(3, 5);

    BSP_SCHEMATIC_TESTS(
      ((s)       ("(3 5)"))
      ((*car(s)) ("3")))
    //]
  }
  
  { //[ascii_integer_pair
    schematic s("foo", 17);

    BSP_SCHEMATIC_TESTS(
      ((s)       ("(\"foo\" 17)"))
      ((*car(s)) ("\"foo\"")))
    //]
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
