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

using boost::spirit::utf8_symbol_type;
using boost::spirit::binary_string_type;

using prana::utree;
using prana::parse_tree;
using prana::tag::sexpr;
using prana::test::parser_harness;

int main (void) { 
  parser_harness<sexpr> ph;

  ph
    ("#t")
    ("#t", utree(true))

    ("#f")
    ("#f", utree(false))

    ("10")
    ("10", utree(10))

    ("#x10", "16")
    ("#x10", utree(16))

    ("#o10", "8")
    ("#o10", utree(8))

    ("17.5", "17.5")
    ("17.5", utree(17.5))

    ("\"foo\"")
    ("\"foo\"", utree("foo"))
    
    ("bar")
    ("bar", utree(utf8_symbol_type("bar")))
    
    ("b?ar")
    ("b?ar", utree(utf8_symbol_type("b?ar")))
    
    ("matches?")
    ("matches?", utree(utf8_symbol_type("matches?")))

    ("\"\"")
    ("\"\"", utree(""))

    ("\"\\\"\"", "\"\"\"")
    ("\"\\\"\"", utree("\""))

    ("\"a\\\"\"", "\"a\"\"")
    ("\"a\\\"\"", utree("a\""))

    ("\"\\\"b\"", "\"\"b\"")
    ("\"\\\"b\"", utree("\"b"))

    ("\"a\\\"b\"", "\"a\"b\"")
    ("\"a\\\"b\"", utree("a\"b"))

    ("#deadbeef#", utree(binary_string_type("\xde\xad\xbe\xef")))
    ("##", utree(binary_string_type("")))
  ;
  
  return sheol::report_errors();
}
 
