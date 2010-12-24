/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>
#include <boost/spirit/home/prana/schematic/routine/capacity.hpp>

int main (void) {
  using namespace boost::spirit::prana;

  { //[capacity_2_ascii
    schematic s("f");
    
    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("\"f\"")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((capacity(s)) (2)))
    //]
  }
  
  { //[capacity_4_ascii
    schematic s("bar");
    
    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("\"bar\"")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((capacity(s)) (4)))
    //]
  }
  
  { //[capacity_16_ascii
    schematic s("fizzbuzz");

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("\"fizzbuzz\"")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((capacity(s)) (16)))
    //]
  }

  return boost::report_errors();
}
 
