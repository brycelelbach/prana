/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iostream>

#include "harness.hpp"

#include <boost/spirit/home/prana/input/parse_sexpr.hpp>
#include <boost/spirit/home/prana/output/generate_sexpr.hpp>

int main (void) {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_sexpr;
  using boost::spirit::prana::parse_sexpr_list;
  using boost::spirit::prana::generate_sexpr;
  using boost::spirit::prana::generate_sexpr_list;

  { //[nil
    std::string in = "nil";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("nil")))
    //]  
  }

  { //[boolean
    std::string in = "#t";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("#t")))
    //]  
  }

  { //[integer_dec
    std::string in = "10";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("10")))
    //]  
  }
  
  { //[integer_hex
    std::string in = "#x10";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("16")))
    //]  
  }
  
  { //[integer_oct
    std::string in = "#o10";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("8")))
    //]  
  }
  
  { //[floating
    std::string in = "17.5";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("17.5")))
    //]  
  }

  { //[string
    std::string in = "\"foo\"";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("\"foo\"")))
    //]  
  }

  { //[string_double_quote_character
    std::string in = "(\"\\\"\" \"a\\\"\" \"\\\"b\" \"a\\\"b\")";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(\"\\\"\" \"a\\\"\" \"\\\"b\" \"a\\\"b\")")))
    //]  
  }

  { //[binary
    std::string in = "#\xDE\xAD\xBE\xEF#";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("#\xDE\xAD\xBE\xEF#")))
    //]  
  }
  
  { //[binary_hash_character
    std::string in = "(### #\xBE## ##\xEF# #\xBE#\xEF#)";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(### #\xBE## ##\xEF# #\xBE#\xEF#)")))
    //]  
  }

  { //[symbol
    std::string in = "bar";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("bar")))
    //]  
  }
  
  { //[simple_list
    std::string in = "(1 2 3)";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(1 2 3)")))

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
      ((out) ("(nil #f 35 0.57 \"bizz\" #\xDE\xED# sizzle)")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  { //[empty_list
    std::string in = "()";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("()")))

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
      ((out) ("(((1)) (-5003 #t) \"foobar\")")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  { //[program_list
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
 
