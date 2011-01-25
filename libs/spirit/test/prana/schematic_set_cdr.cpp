/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>
#include <boost/spirit/home/prana/schematic/routine/set_car.hpp>

int main (void) { try {
  using boost::spirit::prana::schematic;
  using boost::spirit::prana::set_cdr;

  { 
    schematic s(17, 53.5);
    
    BSP_SCHEMATIC_TESTS(
      ((s) ("(17 53.5)")))

    set_cdr(s, 1);

    BSP_SCHEMATIC_TESTS(
      ((s) ("(17 1)")))
  }
  
  { 
    schematic s("foo", "bar");

    BSP_SCHEMATIC_TESTS(
      ((s) ("(\"foo\" \"bar\")")))

    set_cdr(s, true);

    BSP_SCHEMATIC_TESTS(
      ((s) ("(\"foo\" #t)")))
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }
  
  return boost::report_errors();
}
 
