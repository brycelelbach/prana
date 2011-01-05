/*==============================================================================
    Copyright (c) 2011      Bryce Lelbach

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

  // Ignore the windows UTF8 bom, if it is there.
  ignore_utf8_bom(ifs, "data/itunes_store.json"); 

  // utree is the low-level, generic AST used in Prana.
  utree ast;

  // This is the low-level interface. It takes an input form (a string, stream,
  // or utree holding a string currently) as it's first parameter, and
  // produces an output, storing it in it's second parameter. 
  if (!parse_json(ifs, ast))
    throw parse_exception();

  // json_ir is a DSIR - a domain specific intermediate representation. It's a
  // shallow view into the underlying utree AST. 
  json_ir ir(ast);
 
  std::cout << ir["results"][0]["artworkUrl100"] << std::endl;
  //           ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  // This is actually a proto expression. It will never throw; if the specified
  // JSON node doesn't exist, a sentinel node is returned. The underlying utree
  // type of the sentinel node is invalid_type. Note that the type of the above
  // expression is a Proto expression wrapper, which happens to be streamable.
  // It is additionally convertible to utree and json_ir, and it also provides
  // a get member that will explicitly retrieve the underlying utree node. The
  // output from this example should be: 
  // "http://a1.phobos.apple.com/us/r1000/056/Purple/e2/2b/7a/mzi.opitdvsr.tif"
}
 
