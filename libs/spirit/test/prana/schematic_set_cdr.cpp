/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>
#include <boost/spirit/home/prana/schematic/routine/set_car.hpp>

int main (void) {
  using namespace boost::spirit::prana;

  { 
    schematic s(17, 53.5);
    
    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("(17 53.5)")))

    set_cdr(s, 1);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("(17 1)")))
  }
  
  { 
    schematic s("foo", "bar");

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("(\"foo\" \"bar\")")))

    set_cdr(s, true);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("(\"foo\" #t)")))
  }
  
  return boost::report_errors();
}
 
