/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>
#include <boost/spirit/home/prana/schematic/routine/capacity.hpp>

int main (void) { try {
  using boost::spirit::prana::schematic;
  using boost::spirit::prana::capacity;

  { //[capacity_2_ascii
    schematic s("f");
    
    BSP_SCHEMATIC_TESTS(
      ((s) ("\"f\"")))

    BSP_BOOLEAN_TESTS(
      ((capacity(s)) (2)))
    //]
  }
  
  { //[capacity_4_ascii
    schematic s("bar");
    
    BSP_SCHEMATIC_TESTS(
      ((s) ("\"bar\"")))

    BSP_BOOLEAN_TESTS(
      ((capacity(s)) (4)))
    //]
  }
  
  { //[capacity_16_ascii
    schematic s("fizzbuzz");

    BSP_SCHEMATIC_TESTS(
      ((s) ("\"fizzbuzz\"")))

    BSP_BOOLEAN_TESTS(
      ((capacity(s)) (16)))
    //]
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
