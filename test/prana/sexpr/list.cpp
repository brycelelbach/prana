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
  using boost::spirit::prana::magic::extract_source_location;

  std::cout << "empty list test: " << std::endl; 

  { //[empty_list
    std::string in = "(() ( ))";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(() ())")))
    //]  
  }

  std::cout << std::endl << "basic list test: " << std::endl; 

  { //[basic_list
    std::string in = "(1 2 3)";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) (in)))
    //]  
  }
  
  std::cout << std::endl << "list skipping test: " << std::endl; 

  { //[list_skipping
    std::string in = "(1  2   3    4)";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(1 2 3 4)")))
    //]  
  }

  std::cout << std::endl << "multi type list test: " << std::endl; 

  { //[multi_type_list
    std::string in = "(nil #f 35 0.57 \"bizz\" #\xDE\xED# sizzle)";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) (in)))
    //]  
  }
  
  std::cout << std::endl << "nested list test: " << std::endl; 

  { //[nested_list
    std::string in = "(((1)) (-5003 #t) \"foobar\")";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) (in)))
    //]  
  }
  
  std::cout << std::endl << "position test: " << std::endl; 

  { //[position
    std::string in = "   (1\n(2 3)\n)";

    parse_tree<sexpr> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(1 (2 3))")))

    BSP_BOOLEAN_TESTS(
      ((extract_source_location(pt.ast()[0], pt).column()) (3))
      ((extract_source_location(pt.ast()[0], pt).line())   (1))
      ((extract_source_location(pt.ast()[1], pt).line())   (2)))
    //]  
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }
  
  return boost::report_errors();
}
 
