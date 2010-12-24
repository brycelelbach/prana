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

  { //[null
    std::string in = "null";
    utree out;

    parse_json(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))
    //]  
  }

  { //[boolean
    std::string in = "true";
    utree out;

    parse_json(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))
    //]  
  }

  { //[integer
    std::string in = "-19";
    utree out;

    parse_json(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))
    //]  
  }
  
  { //[floating
    std::string in = "4532.5";
    utree out;

    parse_json(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))
    //]  
  }

  { //[string
    std::string in = "\"abc\"";
    utree out;

    parse_json(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))
    //]  
 }

  return boost::report_errors();
}
 
