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

  std::cout << "empty array test: " << std::endl;

  { //[empty_array
    std::string in = "[[], [ ]]";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) ("[[], []]")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }

  std::cout << std::endl << "basic array test: " << std::endl; 

  { //[basic_array
    std::string in = "[null, null, null]";
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
  
  std::cout << std::endl << "array skipping test: " << std::endl; 
  
  { //[array_skipping
    std::string in = "[\"a\",\"b\", \"c\",  \"d\"]\n";
    utree out;

    parse_json(in, out);
    
    std::cout << out << std::endl;
    
    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) ("[\"a\", \"b\", \"c\", \"d\"]")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  std::cout << std::endl << "multi type array test: " << std::endl; 
  
  { //[multi_type_array
    std::string in = "[13053, null, 16.3, \"xyz\", false]";
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
  
  std::cout << std::endl << "nested array test: " << std::endl; 
  
  { //[nested_array
    std::string in = "[[-1, -2, -3], true, [[[\"abc\"]]]]";
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
    std::string in = "[[true, false],\ntrue\n]";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;
    
    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) ("[[true, false], true]")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag())    (1))
      ((out[1].tag()) (2)))
    //]  
  }

  return boost::report_errors();
}
 
