/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>
#include <boost/spirit/home/prana/schematic/routine/size.hpp>

int main (void) { try {
  using boost::spirit::prana::schematic;
  using boost::spirit::prana::size;

  { /*[size_0_ascii*/
    schematic s0("");
  
    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s0) ("\"\"")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((size(s0)) (0)))
  /*]*/ }
  
  { /*[size_3_ascii*/
  schematic s1("bar");
  
  BOOST_SPIRIT_PRANA_SEXPR_TESTS(
    ((s1) ("\"bar\"")))

  BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
    ((size(s1)) (3)))
  /*]*/ }
  
  { /*[size_8_ascii*/
  schematic s2("fizzbuzz");
  
  BOOST_SPIRIT_PRANA_SEXPR_TESTS(
    ((s2) ("\"fizzbuzz\"")))

  BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
    ((size(s2)) (8)))
  /*]*/ }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
