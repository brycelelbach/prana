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
  using boost::spirit::prana::set_car;

  { 
    schematic s(true, false);
    
    BOOST_SPIRIT_PRANA_SCHEMATIC_TESTS(
      ((s) ("(#t #f)")))

    set_car(s, 4);

    BOOST_SPIRIT_PRANA_SCHEMATIC_TESTS(
      ((s) ("(4 #f)")))
  }
  
  { 
    schematic s("pingpong", 312.12);

    BOOST_SPIRIT_PRANA_SCHEMATIC_TESTS(
      ((s) ("(\"pingpong\" 312.12)")))

    set_car(s, "fizz");

    BOOST_SPIRIT_PRANA_SCHEMATIC_TESTS(
      ((s) ("(\"fizz\" 312.12)")))
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }
  
  return boost::report_errors();
}
 
