/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <prana/test/parser_harness.hpp>
#include <prana/parse/parse_tree.hpp>
#include <prana/parse/grammar/sexpr.hpp>
#include <prana/generate/generate_sexpr.hpp>
#include <prana/utree/io.hpp>
#include <prana/utree/make_list.hpp>  

using boost::spirit::nil;
using boost::spirit::utf8_symbol_type;
using boost::spirit::binary_string_type;

using prana::utree;
using prana::parse_tree;
using prana::tag::sexpr;
using prana::magic::extract_source_location;
using prana::make_list;
using prana::test::parser_harness;

int main (void) {
  parser_harness<sexpr> ph;

  ph
    ("(() ( ))", "(() ())")
    ("(() ( ))", make_list(make_list(), make_list()))

    ("(1 2 3)", "(1 2 3)")

    ("(1  2   3    4)", "(1 2 3 4)")
    ("(1  2   3    4)", make_list(1, 2, 3, 4))

    ("(nil #f 35 0.57 \"bizz\" #deed# sizzle)",
      make_list(nil, false, 35, 0.57, "bizz",
        binary_string_type("\xde\xed"), utf8_symbol_type("sizzle")))

    ("(((1)) (-5003 #t) \"foobar\")")
    ("(((1)) (-5003 #t) \"foobar\")",
      make_list(make_list(make_list(1)), make_list(-5003, true), "foobar"))
  ;

  { 
    const std::string in = "   (1\n(2 3)\n)";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt, "(1 (2 3))");
    SHEOL_TEST_EQ(extract_source_location(pt.ast(), pt).column, 4U);
    SHEOL_TEST_EQ(extract_source_location(pt.ast(), pt).line, 1U);
    SHEOL_TEST_EQ(extract_source_location(pt.ast().back(), pt).line, 2U);
  }
  
  return sheol::report_errors();
}
 
