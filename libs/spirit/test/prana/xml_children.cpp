/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/parse/parse_tree.hpp>
#include <boost/spirit/home/prana/parse/grammar/xml.hpp>
#include <boost/spirit/home/prana/generate/generate_sexpr.hpp>

int main (void) { try {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_tree;
  using boost::spirit::prana::tag::xml;
  using boost::spirit::prana::generate_sexpr;

  std::cout << "basic children test: " << std::endl;

  { //[basic_children
    std::string in = "<abc>\n"
                     "  <d>1</d>\n"
                     "  <e>2</e>\n"
                     "  <f>3</f>\n"
                     "</abc>\n";

    parse_tree<xml> pt(in);

    std::cout << pt.ast() << std::endl;

    BSP_STRINGIFY_TESTS(
      generate_sexpr,
      ((pt.ast()) ("(abc () ((d () (1)) (e () (2)) (f () (3))))")))
    //]  
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
