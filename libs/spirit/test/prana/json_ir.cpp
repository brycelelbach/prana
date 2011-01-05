/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/input/parse_json.hpp>
#include <boost/spirit/home/prana/output/generate_json.hpp>
#include <boost/spirit/home/prana/dsir/json/ir.hpp>

int main (void) { try { 
  using boost::spirit::utree;
  using boost::spirit::prana::json_ir;
  using boost::spirit::prana::parse_json;
  using boost::spirit::prana::generate_json;

  std::cout << "instantiation test: " << std::endl;

  { 
    json_ir ir;
  }
  
  std::cout << std::endl << "basic array test: " << std::endl; 
  
  { 
    std::string in = "[[1, 2], [3]]";
    utree out;

    parse_json(in, out);
    
    BOOST_SPIRIT_PRANA_UTREE_TESTS(
      generate_json,
      ((out) (in)))

    json_ir ir(out);

    std::cout << ir[0][1] << std::endl;
  }

  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
