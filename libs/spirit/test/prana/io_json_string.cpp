/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/input/parse_json.hpp>
#include <boost/spirit/home/prana/output/generate_sexpr.hpp>

int main (void) {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_json;
  using boost::spirit::prana::generate_sexpr;

  { //[null
    std::string in = "null";
    utree out;

    parse_json(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("nil")))
    //]  
  }

  { //[boolean
    std::string in = "false";
    utree out;

    parse_json(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("#f")))
    //]  
  }

  { //[integer
    std::string in = "276";
    utree out;

    parse_json(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("276")))
    //]  
  }
  
  { //[floating
    std::string in = "3.14";
    utree out;

    parse_json(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("3.14")))
    //]  
  }

  { //[string
    std::string in = "\"fizz\"";
    utree out;

    parse_json(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("\"fizz\"")))
    //]  
  }

  return boost::report_errors();
}
 
