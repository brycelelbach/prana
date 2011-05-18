/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/parse/parse_tree.hpp>
#include <boost/spirit/home/prana/parse/grammar/xml.hpp>
#include <boost/spirit/home/prana/generate/generate_sexpr.hpp>

int main (void) { try {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_tree;
  using boost::spirit::prana::tag::xml;
  using boost::spirit::prana::generate_sexpr;

  std::cout << "empty test: " << std::endl;

  { //[empty
    std::string in = "<abc />";

    parse_tree<xml> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(abc ())")))
    //]  
  }
  
  std::cout << std::endl << "childless test: " << std::endl;
  
  { //[childless
    std::string in = "<abc></abc>";

    parse_tree<xml> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(abc () ())")))
    //]  
  }
  
  std::cout << std::endl << "boolean test: " << std::endl;

  { //[boolean
    std::string in = "<abc>true</abc>";

    parse_tree<xml> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(abc () (#t))")))
    //]  
  }

  std::cout << std::endl << "integer test: " << std::endl;

  { //[integer
    std::string in = "<abc>123</abc>";

    parse_tree<xml> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(abc () (123))")))
    //]  
  }
  
  std::cout << std::endl << "floating test: " << std::endl;

  { //[floating
    std::string in = "<abc>1.23</abc>";

    parse_tree<xml> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(abc () (1.23))")))
    //]  
  }

  std::cout << std::endl << "string test: " << std::endl;

  { //[string
    std::string in = "<abc>def</abc>";

    parse_tree<xml> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(abc () (\"def\"))")))
    //]  
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
