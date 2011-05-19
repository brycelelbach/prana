////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach 
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <phxpr/config.hpp>

#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>

#include <prana/version.hpp>

#include <phxpr/version.hpp>

#include <sheol/adt/dynamic_array.hpp>

#include <prana/parse/parse_tree.hpp>
#include <prana/parse/grammar/sexpr.hpp>
#include <prana/generate/generate_sexpr.hpp>
#include <prana/utree/io.hpp>
    
#include <phxpr/evaluator.hpp>

using boost::program_options::variables_map;
using boost::program_options::options_description;
using boost::program_options::positional_options_description;
using boost::program_options::value;
using boost::program_options::store;
using boost::program_options::command_line_parser;
using boost::program_options::notify;

using boost::spirit::nil;
using boost::spirit::utree;
using boost::spirit::utree_type;

using sheol::adt::dynamic_array;  

using prana::parse_tree;
using prana::tag::sexpr;
using prana::generate_sexpr;

using phxpr::evaluator;

int main (int argc, char** argv) {
  variables_map vm;

  options_description visible("Usage: phxi [options]"), hidden;
   
  std::string input(""), args("");
 
  visible.add_options()
    ("help,h", "print this message")
    ("version,v", "display the version and copyright information")
  ;

  hidden.add_options()
    ("input", value<std::string>(&input), 
     "input to read and execute phxpr code from") 
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

  else if (vm.count("version")) {
    std::cout <<
      "phxi, the phxpr interpreter (v" PHXPR_VERSION_STR "\n"
      "Prana v" PRANA_VERSION_STR "\n"
      "\n"
      "Copyright (c) 2010-2011 Bryce Lelbach, Joel de Guzman and Hartmut Kaiser\n"
      "\n" 
      "Distributed under the Boost Software License, Version 1.0. (See accompanying\n"
      "file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)\n";
    return 0;
  }

  if (!vm.count("input")) {
    std::cerr << "error: no file specified\n";
    return 1; 
  }

  std::ifstream ifs(input.c_str(), std::ifstream::in);  
  evaluator e;
  
  dynamic_array<boost::shared_ptr<parse_tree<sexpr> > > asts(16);
  utree r;

  // interpreter REL
  while (ifs.good()) {
    asts.push_back(boost::shared_ptr<parse_tree<sexpr> >());

    // read
    asts.back().reset(new parse_tree<sexpr>(ifs));

    // eval
    if (!ifs.good()) {
      r = e(asts.back()->ast());
      break;
    }

    // invoke for side effects
    else 
      e(asts.back()->ast());
  }

  // print the result  
  std::cout << "return value: ";
  generate_sexpr(r, std::cout);
  std::cout << std::endl;

  return 0; 
}
 
