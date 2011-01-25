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
  using boost::spirit::prana::traits::extract_source_location;

  std::cout << "empty array test: " << std::endl;

  { 
    std::string in = "[[], [ ]]";

    parse_tree<json> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt.ast()) ("[[], []]")))
  }

  std::cout << std::endl << "basic array test: " << std::endl; 

  { 
    std::string in = "[null, null, null]";

    parse_tree<json> pt(in);
    
    std::cout << pt.ast() << std::endl;
    
    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt.ast()) (in)))
  }
  
  std::cout << std::endl << "array skipping test: " << std::endl; 
  
  { 
    std::string in = "[\"a\",\"b\", \"c\",  \"d\"]\n";

    parse_tree<json> pt(in);
    
    std::cout << pt.ast() << std::endl;
    
    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt.ast()) ("[\"a\", \"b\", \"c\", \"d\"]")))
  }
  
  std::cout << std::endl << "multi type array test: " << std::endl; 
  
  { 
    std::string in = "[13053, null, 16.3, \"xyz\", false]";

    parse_tree<json> pt(in);

    std::cout << pt.ast() << std::endl;
    
    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt.ast()) (in)))
  }
  
  std::cout << std::endl << "nested array test: " << std::endl; 
  
  { 
    std::string in = "[[-1, -2, -3], true, [[[\"abc\"]]]]";

    parse_tree<json> pt(in);

    std::cout << pt.ast() << std::endl;
    
    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt.ast()) (in)))
  }
  
  std::cout << std::endl << "line position test: " << std::endl; 

  { 
    std::string in = "[[true, false],\ntrue\n]";

    parse_tree<json> pt(in);

    std::cout << pt.ast() << std::endl;
    
    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt.ast()) ("[[true, false], true]")))

    BSP_BOOLEAN_TESTS(
      ((extract_source_location(pt.ast()[0], pt).line()) (1))
      ((extract_source_location(pt.ast()[1], pt).line()) (2)))
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
