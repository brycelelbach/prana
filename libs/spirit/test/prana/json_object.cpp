/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/input/parse_json.hpp>
#include <boost/spirit/home/prana/output/generate_json.hpp>

int main (void) {
  using boost::spirit::utree;
  using boost::spirit::prana::parse_json;
  using boost::spirit::prana::parse_json_object;
  using boost::spirit::prana::generate_json;
  using boost::spirit::prana::generate_json_object;

  { //[empty_object
    std::string in = "[{\n}, {}]";
    utree out;

    parse_json(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) ("[{}, {}]")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  { //[object_list
    std::string in = "{\"foo\":45.4, \"bar\":3.14, \"jazz\":17.17}";
    utree out;

    parse_json(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  { //[object_skipping
    std::string in = "{\"/\":273,\"+\": 36, \"-\" :-17,  \"*\" : 45}\n";
    utree out;

    parse_json(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) ("{\"/\":273, \"+\":36, \"-\": -17, \"*\":45}")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  { //[multi_type_object
    std::string in = "{\"a\":18, \"b\":true, \"c\":null,"
                      "\"d\":0.57, \"e\":\"zoo\", \"f\":[2, 4]}";
    utree out;

    parse_json(in, out);

    std::cout << out << "\n";

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  { //[nested_object
    std::string in = "{\"a\":{\"a\":{\"a\":{\"a\":true}}},"
                      "\"b\":null,"
                      "\"c\":{\"a\":\"waze\",\"b\":451}}";

    utree out;

    parse_json(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag()) (1)))
    //]  
  }
  
  { //[parse_object
    std::string in = "\"car\":1,\n\"cdr\":{\"two\":2, \"three\":3}\n";
    utree out;

    parse_json_object(in, out);

    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json_object,
      ((out) ("{\"car\":1, \"cdr\":{\"two\":2, \"three\":3}}")))

    BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
      ((out.tag())    (1))
      ((out[1].tag()) (2)))
    //]  
  }

  return boost::report_errors();
}
 
