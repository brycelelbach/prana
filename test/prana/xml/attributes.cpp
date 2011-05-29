/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

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
    ("<abc def=\"true\" />")
    ("<abc def=\"true\" />",
      make_list(utf8_symbol_type("abc"),
        make_list(make_list(utf8_symbol_type("def"), true))))

    ("<abc def=\"123\" />")
    ("<abc def=\"123\" />",
      make_list(utf8_symbol_type("abc"),
        make_list(make_list(utf8_symbol_type("def"), 123))))

    ("<abc def=\"1.23\" />")
    ("<abc def=\"1.23\" />",
      make_list(utf8_symbol_type("abc"),
        make_list(make_list(utf8_symbol_type("def"), 1.23))))

    ("<abc def=\"ghi\" />")
    ("<abc def=\"ghi\" />",
      make_list(utf8_symbol_type("abc"),
        make_list(make_list(utf8_symbol_type("def"), "ghi"))))
    
    ("<abc d=\"1\" e=\"0.5\" f=\"bar\" />")
    ("<abc d=\"1\" e=\"0.5\" f=\"bar\" />",
      make_list(utf8_symbol_type("abc"),
        make_list(make_list(utf8_symbol_type("d"), 1),
                  make_list(utf8_symbol_type("e"), 0.5),
                  make_list(utf8_symbol_type("f"), "bar"))))
    
    ("<abc def=\"123\">foo</abc>")
    ("<abc def=\"123\">foo</abc>",
      make_list(utf8_symbol_type("abc"),
        make_list(make_list(utf8_symbol_type("def"), 123)),
        make_list("foo")))
    
    ("<abc d=\"1\" e=\"0.5\" f=\"bar\">foo</abc>")
    ("<abc d=\"1\" e=\"0.5\" f=\"bar\">foo</abc>",
      make_list(utf8_symbol_type("abc"),
        make_list(make_list(utf8_symbol_type("d"), 1),
                  make_list(utf8_symbol_type("e"), 0.5),
                  make_list(utf8_symbol_type("f"), "bar")),
        make_list("foo")))
  ;

  return sheol::report_errors();
}
 
