/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>
#include <boost/spirit/home/prana/schematic/routine/set.hpp>

int main (void) { try {
  using boost::spirit::prana::schematic;
  using boost::spirit::prana::set;

  { //[string_literal
    schematic s;
    
    set(s, "abc");

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("\"abc\"")))
    //]
  }
  
  { //[integer_literal
    schematic s;
    
    set(s, 5);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("5")))
    //]
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }
  
  return boost::report_errors();
}
 
