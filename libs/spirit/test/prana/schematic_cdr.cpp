/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>
#include <boost/spirit/home/prana/schematic/routine/cdr.hpp>

int main (void) { try {
  using boost::spirit::prana::schematic;
  using boost::spirit::prana::cdr;

  { //[floating_boolean_pair
    schematic s(19.95, true);

    BSP_SCHEMATIC_TESTS(
      ((s)       ("(19.95 #t)"))
      ((*cdr(s)) ("#t")))
    //]
  }
  
  { //[ascii_ascii_pair
    schematic s("bar", "bizz");

    BSP_SCHEMATIC_TESTS(
      ((s)       ("(\"bar\" \"bizz\")"))
      ((*cdr(s)) ("\"bizz\"")))
    //]
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }
  
  return boost::report_errors();
}
 
