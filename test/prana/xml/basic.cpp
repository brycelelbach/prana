/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <prana/test/parser_harness.hpp>
#include <prana/parse/parse_tree.hpp>
#include <prana/parse/grammar/xml.hpp>
#include <prana/generate/generate_xml.hpp>
#include <prana/utree/io.hpp>
#include <prana/utree/make_list.hpp>  

using boost::spirit::utf8_symbol_type;

using prana::utree;
using prana::parse_tree;
using prana::tag::xml;
using prana::make_list;
using prana::test::parser_harness;

int main (void) { 
  parser_harness<xml> ph;

  ph
    ("<abc />")
    ("<abc />", make_list(utf8_symbol_type("abc"), make_list()))

    ("<abc></abc>")
    ("<abc></abc>",
      make_list(utf8_symbol_type("abc"), make_list(), make_list()))

    ("<abc>true</abc>")
    ("<abc>true</abc>",
      make_list(utf8_symbol_type("abc"), make_list(), make_list("true")))

    ("<abc>123</abc>")
    ("<abc>123</abc>",
      make_list(utf8_symbol_type("abc"), make_list(), make_list("123")))

    ("<abc>1.23</abc>")
    ("<abc>1.23</abc>", 
      make_list(utf8_symbol_type("abc"), make_list(), make_list("1.23")))

    ("<abc>def</abc>")
    ("<abc>def</abc>",
      make_list(utf8_symbol_type("abc"), make_list(), make_list("def")))
    
    ("<abc>123 def 1.23</abc>")
    ("<abc>123 def 1.23</abc>",
      make_list(utf8_symbol_type("abc"),
        make_list(), make_list("123 def 1.23")))
  ;

  return sheol::report_errors();
}
 
