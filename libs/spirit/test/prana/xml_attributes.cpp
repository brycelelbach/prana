/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/input/parse_xml.hpp>
#include <boost/spirit/home/prana/output/generate_sexpr.hpp>

int main (void) { try {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_xml;
  using boost::spirit::prana::generate_sexpr;

  std::cout << "boolean test: " << std::endl;

  { //[boolean
    std::string in = "<abc def=\"true\" />";
    utree out;

    parse_xml(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(abc ((def #t)))")))
    //]  
  }

  std::cout << std::endl << "integer test: " << std::endl;

  { //[integer
    std::string in = "<abc def=\"123\" />";
    utree out;

    parse_xml(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(abc ((def 123)))")))
    //]  
  }
  
  std::cout << std::endl << "floating test: " << std::endl;

  { //[floating
    std::string in = "<abc def=\"1.23\" />";
    utree out;

    parse_xml(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(abc ((def 1.23)))")))
    //]  
  }

  std::cout << std::endl << "string test: " << std::endl;

  { //[string
    std::string in = "<abc def=\"ghi\" />";
    utree out;

    parse_xml(in, out);

    std::cout << out << std::endl;

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_sexpr,
      ((out) ("(abc ((def \"ghi\")))")))
    //]  
  }

  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
