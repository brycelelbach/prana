/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/parse/parse_tree.hpp>
#include <boost/spirit/home/prana/parse/grammar/sexpr.hpp>
#include <boost/spirit/home/prana/generate/generate_sexpr.hpp>

int main (void) { try {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_tree;
  using boost::spirit::prana::tag::sexpr;
  using boost::spirit::prana::generate_sexpr;

  std::cout << "nil test: " << std::endl; 

  { //[nil
    std::string in = "nil";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) (in)))
    //]  
  }

  std::cout << std::endl << "boolean test: " << std::endl; 

  { //[boolean
    std::string in = "#t";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) (in)))
    //]  
  }

  std::cout << std::endl << "integer dec test: " << std::endl; 

  { //[integer_dec
    std::string in = "10";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) (in)))
    //]  
  }
  
  std::cout << std::endl << "integer hex test: " << std::endl; 

  { //[integer_hex
    std::string in = "#x10";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("16")))
    //]  
  }
  
  std::cout << std::endl << "integer oct test: " << std::endl; 

  { //[integer_oct
    std::string in = "#o10";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("8")))
    //]  
  }
  
  std::cout << std::endl << "floating test: " << std::endl; 

  { //[floating
    std::string in = "17.5";

    parse_tree<sexpr> pt(in);

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) (in)))
    //]  
  }

  std::cout << std::endl << "string test: " << std::endl; 

  { //[string
    std::string in = "\"foo\"";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) (in)))
    //]  
  }
  
  std::cout << std::endl << "empty string test: " << std::endl; 

  { //[empty_string
    std::string in = "\"\"";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) (in)))
    //]  
  }

  std::cout << std::endl << "string double-quote character test: " << std::endl; 

  { //[string_double_quote_character
    std::string in = "(\"\\\"\" \"a\\\"\" \"\\\"b\" \"a\\\"b\")";

    parse_tree<sexpr> pt(in);
    
    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(\"\\\"\" \"a\\\"\" \"\\\"b\" \"a\\\"b\")")))
    //]  
  }

  std::cout << std::endl << "binary test: " << std::endl; 

  { //[binary
    std::string in = "#\xDE\xAD\xBE\xEF#";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) (in)))
    //]  
  }

  std::cout << std::endl << "empty binary test: " << std::endl; 

  { //[empty_binary
    std::string in = "##";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) (in)))
    //]  
  }
  
  std::cout << std::endl << "binary hash character test: " << std::endl; 

  { //[binary_hash_character
    std::string in = "(### #\xBE## ##\xEF# #\xBE#\xEF#)";

    parse_tree<sexpr> pt(in);
    
    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) (in)))
    //]  
  }

  std::cout << std::endl << "symbol test: " << std::endl; 

  { //[symbol
    std::string in = "bar";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) (in)))
    //]  
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }
  
  return boost::report_errors();
}
 
