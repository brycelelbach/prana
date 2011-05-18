/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/parse/parse_tree.hpp>
#include <boost/spirit/home/prana/parse/grammar/json.hpp>
#include <boost/spirit/home/prana/generate/generate_json.hpp>

int main (void) { try {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_tree;
  using boost::spirit::prana::tag::json;
  using boost::spirit::prana::generate_json;

  std::cout << "null test: " << std::endl; 

  { 
    std::string in = "null";

    parse_tree<json> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt) (in)))
  }

  std::cout << std::endl << "boolean test: " << std::endl; 

  { 
    std::string in = "true";

    parse_tree<json> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt) (in)))
  }

  std::cout << std::endl << "integer test: " << std::endl; 

  { 
    std::string in = "-19";

    parse_tree<json> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt) (in)))
  }
  
  std::cout << std::endl << "floating test: " << std::endl; 

  {
    std::string in = "4532.5";

    parse_tree<json> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt) (in)))
  }

  std::cout << std::endl << "string test: " << std::endl; 

  { 
    std::string in = "\"abc\"";

    parse_tree<json> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt) (in)))
  }

  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
