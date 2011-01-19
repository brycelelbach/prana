/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/parse/parse_json.hpp>
#include <boost/spirit/home/prana/generate/generate_json.hpp>

int main (void) { try {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_json;
  using boost::spirit::prana::generate_json;

  std::cout << "empty object test: " << std::endl; 

  { 
    std::string in = "[{\n}, {}]";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_STRINGIFY_TESTS(
      generate_json,
      ((out) ("[{}, {}]")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
  }
  
  std::cout << std::endl << "basic object test: " << std::endl; 

  { 
    std::string in = "{\"foo\":45.4, \"bar\":3.14, \"jazz\":17.17}";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_STRINGIFY_TESTS(
      generate_json,
      ((out) (in)))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
  }
  
  std::cout << std::endl << "object skipping test: " << std::endl; 

  { 
    std::string in = "{\"/\":273,\"+\": 36, \"-\" :-17,  \"*\" : 45}\n";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_STRINGIFY_TESTS(
      generate_json,
      ((out) ("{\"/\":273, \"+\":36, \"-\":-17, \"*\":45}")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
  }
  
  std::cout << std::endl << "multi type object test: " << std::endl; 

  { 
    std::string in = "{\"a\":18, \"b\":true, \"c\":null, "
                      "\"d\":0.57, \"e\":\"zoo\", \"f\":[2, 4]}";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_STRINGIFY_TESTS(
      generate_json,
      ((out) (in)))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
  }
  
  std::cout << std::endl << "nested object test: " << std::endl; 

  { 
    std::string in = "{\"a\":{\"a\":{\"a\":{\"a\":true}}}, "
                      "\"b\":null, "
                      "\"c\":{\"a\":\"waze\", \"b\":451}}";

    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_STRINGIFY_TESTS(
      generate_json,
      ((out) (in)))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
  }
  
  std::cout << std::endl << "line position test: " << std::endl; 

  { 
    std::string in = "{\"car\":1,\n\"cdr\":{\"two\":2, \"three\":3}\n}";
    utree out;

    parse_json(in, out);
    
    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_STRINGIFY_TESTS(
      generate_json,
      ((out) ("{\"car\":1, \"cdr\":{\"two\":2, \"three\":3}}")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag())    (1))
      ((out[1].tag()) (2)))
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
