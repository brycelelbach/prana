/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/input/parse_json.hpp>
#include <boost/spirit/home/prana/output/generate_json.hpp>

int main (void) {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_json;
  using boost::spirit::prana::generate_json;

  std::cout << "empty object test: " << std::endl; 

  { //[empty_object
    std::string in = "[{\n}, {}]";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) ("[{}, {}]")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  std::cout << std::endl << "basic object test: " << std::endl; 

  { //[basic_object
    std::string in = "{\"foo\":45.4, \"bar\":3.14, \"jazz\":17.17}";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  std::cout << std::endl << "object skipping test: " << std::endl; 

  { //[object_skipping
    std::string in = "{\"/\":273,\"+\": 36, \"-\" :-17,  \"*\" : 45}\n";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) ("{\"/\":273, \"+\":36, \"-\":-17, \"*\":45}")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  std::cout << std::endl << "multi type object test: " << std::endl; 

  { //[multi_type_object
    std::string in = "{\"a\":18, \"b\":true, \"c\":null, "
                      "\"d\":0.57, \"e\":\"zoo\", \"f\":[2, 4]}";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  std::cout << std::endl << "nested object test: " << std::endl; 

  { //[nested_object
    std::string in = "{\"a\":{\"a\":{\"a\":{\"a\":true}}}, "
                      "\"b\":null, "
                      "\"c\":{\"a\":\"waze\", \"b\":451}}";

    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  std::cout << std::endl << "line position test: " << std::endl; 

  { //[line_position
    std::string in = "{\"car\":1,\n\"cdr\":{\"two\":2, \"three\":3}\n}";
    utree out;

    parse_json(in, out);
    
    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) ("{\"car\":1, \"cdr\":{\"two\":2, \"three\":3}}")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag())    (1))
      ((out[1].tag()) (2)))
    //]  
  }

  return boost::report_errors();
}
 
