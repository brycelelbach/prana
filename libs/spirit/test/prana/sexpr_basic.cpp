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
  using boost::spirit::prana::generate_sexpr;

  { //[nil
    std::string in = "nil";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))
    //]  
  }

  { //[boolean
    std::string in = "#t";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))
    //]  
  }

  { //[integer_dec
    std::string in = "10";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))
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
      ((out) (in)))
    //]  
  }

  { //[string
    std::string in = "\"foo\"";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))
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
      ((out) (in)))
    //]  
  }
  
  { //[binary_hash_character
    std::string in = "(### #\xBE## ##\xEF# #\xBE#\xEF#)";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))
    //]  
  }

  { //[symbol
    std::string in = "bar";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))
    //]  
  }
  
  return boost::report_errors();
}
 
