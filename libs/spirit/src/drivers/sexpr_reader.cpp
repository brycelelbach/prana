////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2011 Bryce Lelbach 
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#include <boost/config.hpp>
#include <boost/program_options.hpp>

#include <boost/spirit/home/prana/parse/parse_tree.hpp>
#include <boost/spirit/home/prana/parse/grammar/sexpr.hpp>
#include <boost/spirit/home/prana/generate/generate_sexpr.hpp>
    
using boost::program_options::variables_map;
using boost::program_options::options_description;
using boost::program_options::positional_options_description;
using boost::program_options::value;
using boost::program_options::store;
using boost::program_options::command_line_parser;
using boost::program_options::notify;
  
using boost::spirit::prana::parse_tree;
using boost::spirit::prana::tag::sexpr;
using boost::spirit::prana::generate_sexpr;

int main (int argc, char** argv) {
  variables_map vm;

  options_description visible("Usage: sexpr_reader [options]"), hidden;
   
  std::string file("");
 
  visible.add_options()
    ("help,h", "print this message")
  ;

  hidden.add_options()
    ("input,i", value<std::string>(&file), 
     "file to read s-expressions from") 
  ;

  positional_options_description p;
  p.add("input", -1);

  options_description cli;
  cli.add(visible).add(hidden);

  store(command_line_parser(argc, argv).options(cli).positional(p).run(), vm);

  notify(vm);

  /////////////////////////////////////////////////////////////////////////////
  // print help screen
  if (vm.count("help")) {
    std::cout << visible;
    return 0;
  }

  if (!vm.count("input")) {
    std::cerr << "error: no file specified\n";
    return 1; 
  }

  try {
    std::ifstream ifs(file.c_str(), std::ifstream::in);  
    parse_tree<sexpr> pt(ifs);
    generate_sexpr(pt.ast(), std::cout);
    std::cout << "\n";
  }

  catch (std::exception& e) {
    std::cerr << "error: caught exception \"" << e.what() << "\"\n";
    return 1;
  }

  catch (...) {
    std::cerr << "error: caught unknown exception\n";
    return 1;
  }

  return 0; 
}
 
