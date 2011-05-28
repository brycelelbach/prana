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
    ("[[], [ ]]", "[[], []]")
    ("[[], [ ]]", make_list(utf8_symbol_type("[]"), utf8_symbol_type("[]")))

    ("[null, null, null]")
    ("[null, null, null]", make_list(nil, nil, nil))

    ("[\"a\",\"b\", \"c\",  \"d\"]\n", "[\"a\", \"b\", \"c\", \"d\"]")
    ("[\"a\",\"b\", \"c\",  \"d\"]\n", make_list("a", "b", "c", "d"))

    ("[13053, null, 16.3, \"xyz\", false]")
    ("[13053, null, 16.3, \"xyz\", false]",
      make_list(13053, nil, 16.3, "xyz", false))

    ("[[-1, -2, -3], true, [[[\"abc\"]]]]")
    ("[[-1, -2, -3], true, [[[\"abc\"]]]]",
      make_list(make_list(-1, -2, -3), true,
        make_list(make_list(make_list("abc")))))
  ;

  { 
    const std::string in = "[[true, false], \n[true]]";
    parse_tree<json> pt(in);
    SHEOL_TEST_STREQ(pt, "[[true, false], [true]]");
    SHEOL_TEST_EQ(extract_source_location(pt.ast().front(), pt).column, 14U); 
    SHEOL_TEST_EQ(extract_source_location(pt.ast().front(), pt).line, 1U); 
    SHEOL_TEST_EQ(extract_source_location(pt.ast().back(), pt).line, 2U); 
  }
  
  return sheol::report_errors();
}
 
