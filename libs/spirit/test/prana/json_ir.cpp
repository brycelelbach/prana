/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/parse/parse_tree.hpp>
#include <boost/spirit/home/prana/parse/grammar/json.hpp>
#include <boost/spirit/home/prana/generate/generate_json.hpp>
#include <boost/spirit/home/prana/dsir/json/ir.hpp>

int main (void) { try { 
  using boost::spirit::utree;
  using boost::spirit::prana::json_ir;
  using boost::spirit::prana::parse_tree;
  using boost::spirit::prana::tag::json;
  using boost::spirit::prana::generate_json;

  std::cout << "basic array test: " << std::endl; 
  
  { 
    std::string in = "[[1, 2], [3]]";

    parse_tree<json> pt(in);
   
    std::cout << pt.ast() << std::endl;
    
    json_ir ir(pt.ast());
 
    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt.ast())         (in))
      (((ir[0]).get())    ("[1, 2]"))
      (((ir[0][0]).get()) ("1"))
      (((ir[0][1]).get()) ("2"))
      (((ir[1]).get())    ("[3]"))
      (((ir[1][0]).get()) ("3"))
      (((ir[2]).get())    ("\"invalid\""))
      (((ir[3][1]).get()) ("\"invalid\""))
      (((ir[0][2]).get()) ("\"invalid\""))
    )
  }
  
  std::cout << "basic array test: " << std::endl; 

  { 
    std::string in = "{\"a\":{\"b\":1, \"c\":2}, \"d\":{\"e\":3}}";

    parse_tree<json> pt(in);
    
    std::cout << pt.ast() << std::endl;

    json_ir ir(pt.ast());
    
    BSP_STRINGIFY_TESTS(
      generate_json,
      ((pt.ast())             (in))
      (((ir["a"]).get())      ("{\"b\":1, \"c\":2}"))
      (((ir["a"]["b"]).get()) ("1"))
      (((ir["a"]["c"]).get()) ("2"))
      (((ir["d"]).get())      ("{\"e\":3}"))
      (((ir["d"]["e"]).get()) ("3"))
      (((ir["f"]).get())      ("\"invalid\""))
      (((ir["g"]["h"]).get()) ("\"invalid\""))
      (((ir["a"]["d"]).get()) ("\"invalid\""))
    )
  }

  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
