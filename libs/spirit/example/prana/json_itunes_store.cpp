/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/spirit/home/prana/input/parse_json.hpp>
#include <boost/spirit/home/prana/output/generate_json.hpp>
#include <boost/spirit/home/prana/dsir/json/ir.hpp>
#include <boost/spirit/home/prana/support/file_io.hpp>

int main (void) { 
  using boost::spirit::utree;
  using boost::spirit::prana::ignore_utf8_bom;
  using boost::spirit::prana::parse_exception;
  using boost::spirit::prana::json_ir;
  using boost::spirit::prana::parse_json;
  using boost::spirit::prana::generate_json;

  std::ifstream ifs("data/itunes_store.json");

  ignore_utf8_bom(ifs, "data/itunes_store.json"); 

  utree ast;

  if (!parse_json(ifs, ast))
    throw parse_exception();
    
  json_ir ir(ast);
 
  std::cout << ir["results"][0]["artworkUrl100"] << std::endl;
  // "http://a1.phobos.apple.com/us/r1000/056/Purple/e2/2b/7a/mzi.opitdvsr.tif"
}
 
