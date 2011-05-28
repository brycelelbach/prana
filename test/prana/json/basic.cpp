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

using boost::spirit::nil;

using prana::utree;
using prana::parse_tree;
using prana::tag::json;
using prana::test::parser_harness;

int main (void) { 
  parser_harness<json> ph;

  ph
    ("null")
    ("null", utree(nil))

    ("true")
    ("true", utree(true))

    ("-19")
    ("-19", utree(-19))

    ("4532.5")
    ("4532.5", utree(4532.5))

    ("\"abc\"")
    ("\"abc\"", utree("abc"))
  ;

  return sheol::report_errors();
}
 
