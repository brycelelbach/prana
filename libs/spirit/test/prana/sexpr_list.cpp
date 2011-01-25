/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/parse/parse_sexpr.hpp>
#include <boost/spirit/home/prana/generate/generate_sexpr.hpp>

int main (void) { try {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_sexpr;
  using boost::spirit::prana::generate_sexpr;

  std::cout << "empty list test: " << std::endl; 

  { //[empty_list
    std::string in = "(() ( ))";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((out) ("(() ())")))

    BSP_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }

  std::cout << std::endl << "basic list test: " << std::endl; 

  { //[basic_list
    std::string in = "(1 2 3)";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((out) (in)))

    BSP_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  std::cout << std::endl << "list skipping test: " << std::endl; 

  { //[list_skipping
    std::string in = "(1  2   3    4)";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((out) ("(1 2 3 4)")))

    BSP_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }

  std::cout << std::endl << "multi type list test: " << std::endl; 

  { //[multi_type_list
    std::string in = "(nil #f 35 0.57 \"bizz\" #\xDE\xED# sizzle)";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((out) (in)))

    BSP_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  std::cout << std::endl << "nested list test: " << std::endl; 

  { //[nested_list
    std::string in = "(((1)) (-5003 #t) \"foobar\")";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((out) (in)))

    BSP_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  std::cout << std::endl << "line position test: " << std::endl; 

  { //[line_position
    std::string in = "(1\n(2 3)\n)";
    utree out;

    parse_sexpr(in, out);

    std::cout << out << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((out) ("(1 (2 3))")))

    BSP_BOOLEAN_TESTS(
      ((out.tag())    (1))
      ((out[1].tag()) (2)))
    //]  
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }
  
  return boost::report_errors();
}
 
