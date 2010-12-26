/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>
#include <boost/spirit/home/prana/schematic/routine/set.hpp>

int main (void) {
  using namespace boost::spirit::prana;

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
  
  return boost::report_errors();
}
 
