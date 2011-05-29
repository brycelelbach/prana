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
    ("<abc><d>1</d><e>2</e><f>3</f></abc>")
    ("<abc><d>1</d><e>2</e><f>3</f></abc>",
      make_list(utf8_symbol_type("abc"), make_list(),
        make_list(
          make_list(utf8_symbol_type("d"), make_list(), make_list("1")),
          make_list(utf8_symbol_type("e"), make_list(), make_list("2")),
          make_list(utf8_symbol_type("f"), make_list(), make_list("3"))
        )))
    
    ("<abc>1<e>2</e>3</abc>")
    ("<abc>1<e>2</e>3</abc>",
      make_list(utf8_symbol_type("abc"), make_list(),
        make_list(
          "1",
          make_list(utf8_symbol_type("e"), make_list(), make_list("2")),
          "3" 
        )))
    
    ("<abc x=\"0\"><d x=\"1\">1</d><e x=\"2\">2</e><f x=\"3\">3</f></abc>") 
    ("<abc x=\"0\"><d x=\"1\">1</d><e x=\"2\">2</e><f x=\"3\">3</f></abc>",
      make_list(utf8_symbol_type("abc"),
        make_list(make_list(utf8_symbol_type("x"), 0)),
        make_list(
          make_list(utf8_symbol_type("d"),
            make_list(make_list(utf8_symbol_type("x"), 1)), make_list("1")),
          make_list(utf8_symbol_type("e"),
            make_list(make_list(utf8_symbol_type("x"), 2)), make_list("2")),
          make_list(utf8_symbol_type("f"),
            make_list(make_list(utf8_symbol_type("x"), 3)), make_list("3")))))
  ;

  return sheol::report_errors();
}
 
