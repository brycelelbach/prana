/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>
#include <boost/spirit/home/prana/schematic/routine/cdr.hpp>

int main (void) {
  using namespace boost::spirit::prana;

  { //[floating_boolean_pair
    schematic s(19.95, true);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s)       ("(19.95 #t)"))
      ((*cdr(s)) ("#t")))
    //]
  }
  
  { //[ascii_ascii_pair
    schematic s("bar", "bizz");

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s)       ("(\"bar\" \"bizz\")"))
      ((*cdr(s)) ("\"bizz\"")))
    //]
  }
  
  return boost::report_errors();
}
 