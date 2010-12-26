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

  std::cout << "null test: " << std::endl; 

  { //[null
    std::string in = "null";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))
    //]  
  }

  std::cout << std::endl << "boolean test: " << std::endl; 

  { //[boolean
    std::string in = "true";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))
    //]  
  }

  std::cout << std::endl << "integer test: " << std::endl; 

  { //[integer
    std::string in = "-19";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))
    //]  
  }
  
  std::cout << std::endl << "floating test: " << std::endl; 

  { //[floating
    std::string in = "4532.5";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))
    //]  
  }

  std::cout << std::endl << "string test: " << std::endl; 

  { //[string
    std::string in = "\"abc\"";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))
    //]  
 }

  return boost::report_errors();
}
 
