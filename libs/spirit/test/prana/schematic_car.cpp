/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>
#include <boost/spirit/home/prana/schematic/routine/car.hpp>

int main (void) {
  using namespace boost::spirit::prana;

  { //[integer_integer_pair
    schematic s(3, 5);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s)       ("(3 5)"))
      ((*car(s)) ("3")))
    //]
  }
  
  { //[ascii_integer_pair
    schematic s("foo", 17);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s)       ("(\"foo\" 17)"))
      ((*car(s)) ("\"foo\"")))
    //]
  }
  
  return boost::report_errors();
}
 
