/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/input/parse_json.hpp>
#include <boost/spirit/home/prana/output/generate_json.hpp>

int main (void) { try {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_json;
  using boost::spirit::prana::generate_json;

  std::cout << "null test: " << std::endl; 

  { 
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

  { 
    std::string in = "true";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))
  }

  std::cout << std::endl << "integer test: " << std::endl; 

  { 
    std::string in = "-19";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))
  }
  
  std::cout << std::endl << "floating test: " << std::endl; 

  {
    std::string in = "4532.5";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))
  }

  std::cout << std::endl << "string test: " << std::endl; 

  { 
    std::string in = "\"abc\"";
    utree out;

    parse_json(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
