/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/input/parse_xml.hpp>
#include <boost/spirit/home/prana/output/generate_sexpr.hpp>

int main (void) {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_xml;
  using boost::spirit::prana::generate_sexpr;

  std::cout << "empty test: " << std::endl;

  { //[empty
    std::string in = "<flop></flop>";
    utree out;

    parse_xml(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(flop)")))
    //]  
  }
  
  std::cout << "boolean test: " << std::endl;

  { //[boolean
    std::string in = "<foo>true</foo>";
    utree out;

    parse_xml(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(foo (#t))")))
    //]  
  }

  std::cout << "integer test: " << std::endl;

  { //[integer
    std::string in = "<bar>5</bar>";
    utree out;

    parse_xml(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(bar (5))")))
    //]  
  }
  
  std::cout << "floating test: " << std::endl;

  { //[floating
    std::string in = "<dab>2145.71</dab>";
    utree out;

    parse_xml(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(dab (2145.71))")))
    //]  
  }

  std::cout << "string test: " << std::endl;

  { //[string
    std::string in = "<bizz>abcd</bizz>";
    utree out;

    parse_xml(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(bizz (\"abcd\"))")))
    //]  
  }

  return boost::report_errors();
}
 
