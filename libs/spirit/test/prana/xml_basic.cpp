/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/input/parse_xml.hpp>
#include <boost/spirit/home/prana/output/generate_sexpr.hpp>

int main (void) {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_xml;
  using boost::spirit::prana::generate_sexpr;

  { //[boolean
    std::string in = "<foo>true</foo>";
    utree out;

    parse_xml(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(foo (#t))")))
    //]  
  }

  { //[integer
    std::string in = "<bar>5</bar>";
    utree out;

    parse_xml(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(bar (5))")))
    //]  
  }
  
  { //[floating
    std::string in = "<dab>2145.71</dab>";
    utree out;

    parse_xml(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(dab (2145.71))")))
    //]  
  }

  { //[string
    std::string in = "<bizz>abcd</bizz>";
    utree out;

    parse_xml(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(bizz (\"abcd\"))")))
    //]  
  }

  { //[symbol
    std::string in = "<flop></flop>";
    utree out;

    parse_xml(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(flop)")))
    //]  
  }
  
  return boost::report_errors();
}
 
