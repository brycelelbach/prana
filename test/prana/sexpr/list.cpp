/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <sheol/lightweight_test.hpp>

#include <prana/parse/parse_tree.hpp>
#include <prana/parse/grammar/sexpr.hpp>
#include <prana/utree/io.hpp>

int main (void) {
  using boost::spirit::utree;

  using prana::parse_tree;
  using prana::tag::sexpr;
  using prana::generate_sexpr;
  using prana::magic::extract_source_location;

  { 
    const std::string in = "(() ( ))";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "(() ())");
  }

  { 
    const std::string in = "(1 2 3)";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "(1 2 3)");
  }

  { 
    const std::string in = "(1  2   3    4)";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "(1 2 3 4)");
  }

  { 
    const std::string in = "(nil #f 35 0.57 \"bizz\" #\xDE\xED# sizzle)";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "(nil #f 35 0.57 \"bizz\" #\xDE\xED# sizzle)");
  }

  { 
    const std::string in = "(((1)) (-5003 #t) \"foobar\")";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "(((1)) (-5003 #t) \"foobar\")");
  }

  { 
    const std::string in = "   (1\n(2 3)\n)";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "(1 (2 3))");
    SHEOL_TEST_EQ(extract_source_location(pt.ast(), pt).column, 4U);
    SHEOL_TEST_EQ(extract_source_location(pt.ast(), pt).line, 1U);
    SHEOL_TEST_EQ(extract_source_location(pt.ast().back(), pt).line, 2U);
  }
  
  return sheol::report_errors();
}
 
