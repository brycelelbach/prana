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

  std::cout << "empty object test: " << std::endl; 

  { 
    std::string in = "[{\n}, {}]";

    parse_tree<json> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt) ("[{}, {}]")))
  }
  
  std::cout << std::endl << "basic object test: " << std::endl; 

  { 
    std::string in = "{\"foo\":45.4, \"bar\":3.14, \"jazz\":17.17}";

    parse_tree<json> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt) (in)))
  }
  
  std::cout << std::endl << "object skipping test: " << std::endl; 

  { 
    std::string in = "{\"/\":273,\"+\": 36, \"-\" :-17,  \"*\" : 45}\n";

    parse_tree<json> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt) ("{\"/\":273, \"+\":36, \"-\":-17, \"*\":45}")))
  }
  
  std::cout << std::endl << "multi type object test: " << std::endl; 

  { 
    std::string in = "{\"a\":18, \"b\":true, \"c\":null, "
                      "\"d\":0.57, \"e\":\"zoo\", \"f\":[2, 4]}";

    parse_tree<json> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt) (in)))
  }
  
  std::cout << std::endl << "nested object test: " << std::endl; 

  { 
    std::string in = "{\"a\":{\"a\":{\"a\":{\"a\":true}}}, "
                      "\"b\":null, "
                      "\"c\":{\"a\":\"waze\", \"b\":451}}";


    parse_tree<json> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt) (in)))
  }
  
  std::cout << std::endl << "line position test: " << std::endl; 

  { 
    std::string in = "{\"car\":1,\n\"cdr\":{\"two\":2, \"three\":3}\n}";

    parse_tree<json> pt(in);
    
    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt) ("{\"car\":1, \"cdr\":{\"two\":2, \"three\":3}}")))

    BSP_BOOLEAN_TESTS(
      ((extract_source_location(pt.ast()[0], pt).line()) (1))
      ((extract_source_location(pt.ast()[0], pt).line()) (2)))
  }

  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
