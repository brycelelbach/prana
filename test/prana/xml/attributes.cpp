/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iostream>

#include <prana/parse/parse_tree.hpp>
#include <prana/parse/grammar/xml.hpp>
#include <prana/generate/generate_xml.hpp>
  
using prana::utree;
using prana::parse_tree;
using prana::tag::xml;
using prana::generate_xml;

int main (void) { 
  std::string in = "<abc def=\"true\" />";

  parse_tree<xml> pt(in);

  std::cout << pt << std::endl;

#if 0
  std::cout << "boolean test: " << std::endl;

  { //[boolean
    std::string in = "<abc def=\"true\" />";

    parse_tree<xml> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(abc ((def #t)))")))
    //]  
  }

  std::cout << std::endl << "integer test: " << std::endl;

  { //[integer
    std::string in = "<abc def=\"123\" />";

    parse_tree<xml> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(abc ((def 123)))")))
    //]  
  }
  
  std::cout << std::endl << "floating test: " << std::endl;

  { //[floating
    std::string in = "<abc def=\"1.23\" />";

    parse_tree<xml> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(abc ((def 1.23)))")))
    //]  
  }

  std::cout << std::endl << "string test: " << std::endl;

  { //[string
    std::string in = "<abc def=\"ghi\" />";

    parse_tree<xml> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(abc ((def \"ghi\")))")))
    //]  
  }

  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
#endif
}
 
