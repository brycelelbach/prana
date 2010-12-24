/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/input/parse_sexpr.hpp>
#include <boost/spirit/home/prana/output/generate_sexpr.hpp>

int main (void) {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_sexpr;
  using boost::spirit::prana::parse_sexpr_list;
  using boost::spirit::prana::generate_sexpr;
  using boost::spirit::prana::generate_sexpr_list;

  { //[empty_list
    std::string in = "(() ( ))";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(() ())")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  { //[simple_list
    std::string in = "(1 2 3)";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  { //[list_skipping
    std::string in = "(1  2   3    4)";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(1 2 3 4)")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }

  { //[multi_type_list
    std::string in = "(nil #f 35 0.57 \"bizz\" #\xDE\xED# sizzle)";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  { //[nested_list
    std::string in = "(((1)) (-5003 #t) \"foobar\")";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  { //[parse_list
    std::string in = "1\n(2 3)\n";
    utree out;

    parse_sexpr_list(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr_list,
      ((out) ("(1 (2 3))")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag())    (1))
      ((out[1].tag()) (2)))
    //]  
  }
  
  return boost::report_errors();
}
 
