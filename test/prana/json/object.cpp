/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <prana/test/parser_harness.hpp>
#include <prana/parse/parse_tree.hpp>
#include <prana/parse/grammar/json.hpp>
#include <prana/generate/generate_json.hpp>
#include <prana/utree/io.hpp>
#include <prana/utree/make_list.hpp>  

using boost::spirit::nil;
using boost::spirit::utf8_symbol_type;

using prana::utree;
using prana::parse_tree;
using prana::tag::json;
using prana::magic::extract_source_location;
using prana::make_list;
using prana::test::parser_harness;

int main (void) { 
  parser_harness<json> ph;

  ph
    ("[{\n}, {}]", "[{}, {}]")

    ("{\"foo\":45.4, \"bar\":3.14, \"jazz\":17.17}")

    ("{\"/\":273,\"+\": 36, \"-\" :-17,  \"*\" : 45}\n",
     "{\"/\":273, \"+\":36, \"-\":-17, \"*\":45}")

    ("{\"a\":18, \"b\":true, \"c\":null, "
     "\"d\":0.57, \"e\":\"zoo\", \"f\":[2, 4]}")

    ("{\"a\":{\"a\":{\"a\":{\"a\":true}}}, "
     "\"b\":null, "
     "\"c\":{\"a\":\"waze\", \"b\":451}}")
  ;

  { 
    const std::string in = "{\"car\":1,\n\"cdr\":{\"two\":2, \"three\":3}}";
    parse_tree<json> pt(in);
    SHEOL_TEST_STREQ(pt, "{\"car\":1, \"cdr\":{\"two\":2, \"three\":3}}");
    SHEOL_TEST_EQ(extract_source_location(pt.ast().front().front(), pt).column, 8U); 
    SHEOL_TEST_EQ(extract_source_location(pt.ast().front().front(), pt).line, 1U); 
    SHEOL_TEST_EQ(extract_source_location(pt.ast().back().back(), pt).line, 2U); 
  }

  return sheol::report_errors();
}

