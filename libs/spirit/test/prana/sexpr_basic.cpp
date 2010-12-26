/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/input/parse_sexpr.hpp>
#include <boost/spirit/home/prana/output/generate_sexpr.hpp>

int main (void) {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_sexpr;
  using boost::spirit::prana::generate_sexpr;

  std::cout << "nil test: " << std::endl; 

  { //[nil
    std::string in = "nil";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))
    //]  
  }

  std::cout << std::endl << "boolean test: " << std::endl; 

  { //[boolean
    std::string in = "#t";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))
    //]  
  }

  std::cout << std::endl << "integer dec test: " << std::endl; 

  { //[integer_dec
    std::string in = "10";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))
    //]  
  }
  
  std::cout << std::endl << "integer hex test: " << std::endl; 

  { //[integer_hex
    std::string in = "#x10";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("16")))
    //]  
  }
  
  std::cout << std::endl << "integer oct test: " << std::endl; 

  { //[integer_oct
    std::string in = "#o10";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("8")))
    //]  
  }
  
  std::cout << std::endl << "floating test: " << std::endl; 

  { //[floating
    std::string in = "17.5";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))
    //]  
  }

  std::cout << std::endl << "string test: " << std::endl; 

  { //[string
    std::string in = "\"foo\"";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))
    //]  
  }

  std::cout << std::endl << "string double-quote character test: " << std::endl; 

  { //[string_double_quote_character
    std::string in = "(\"\\\"\" \"a\\\"\" \"\\\"b\" \"a\\\"b\")";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(\"\\\"\" \"a\\\"\" \"\\\"b\" \"a\\\"b\")")))
    //]  
  }

  std::cout << std::endl << "binary test: " << std::endl; 

  { //[binary
    std::string in = "#\xDE\xAD\xBE\xEF#";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))
    //]  
  }
  
  std::cout << std::endl << "binary hash character test: " << std::endl; 

  { //[binary_hash_character
    std::string in = "(### #\xBE## ##\xEF# #\xBE#\xEF#)";
    utree out;

    parse_sexpr(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))
    //]  
  }

  std::cout << std::endl << "symbol test: " << std::endl; 

  { //[symbol
    std::string in = "bar";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) (in)))
    //]  
  }
  
  return boost::report_errors();
}
 
