////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach 
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#include <boost/config.hpp>
#include <boost/program_options.hpp>

#include <boost/spirit/home/prana/version.hpp>
#include <boost/spirit/home/prana/adt/dynamic_array.hpp>
#include <boost/spirit/home/prana/parse/parse_tree.hpp>
#include <boost/spirit/home/prana/parse/grammar/sexpr.hpp>
#include <boost/spirit/home/prana/generate/generate_sexpr.hpp>
#include <boost/spirit/home/prana/phxpr/evaluator.hpp>
#include <boost/spirit/home/prana/phxpr/core.hpp>
#include <boost/spirit/home/prana/phxpr/intrinsics.hpp>
    
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

using boost::spirit::prana::dynamic_array;  
using boost::spirit::prana::parse_tree;
using boost::spirit::prana::tag::sexpr;
using boost::spirit::prana::generate_sexpr;

using boost::spirit::prana::phxpr::evaluator;
using boost::spirit::prana::phxpr::function;
using boost::spirit::prana::phxpr::procedure;
using boost::spirit::prana::phxpr::quote;

using boost::spirit::prana::phxpr::begin;
using boost::spirit::prana::phxpr::at;
using boost::spirit::prana::phxpr::lt;
using boost::spirit::prana::phxpr::lte;
using boost::spirit::prana::phxpr::and_;
using boost::spirit::prana::phxpr::or_;
using boost::spirit::prana::phxpr::if_;
using boost::spirit::prana::phxpr::list;
using boost::spirit::prana::phxpr::add;
using boost::spirit::prana::phxpr::min;
using boost::spirit::prana::phxpr::mul;
using boost::spirit::prana::phxpr::div_;
using boost::spirit::prana::phxpr::display;
using boost::spirit::prana::phxpr::front;
using boost::spirit::prana::phxpr::back;
using boost::spirit::prana::phxpr::rest;
using boost::spirit::prana::phxpr::not_;
using boost::spirit::prana::phxpr::empty;
using boost::spirit::prana::phxpr::push_front;
using boost::spirit::prana::phxpr::push_back;
using boost::spirit::prana::phxpr::newline;

int main (int argc, char** argv) {
  variables_map vm;

  options_description visible("Usage: phxi [options]"), hidden;
   
  std::string input(""), args("");
 
  visible.add_options()
    ("help,h", "print this message")
    ("version,v", "display the version and copyright information")
    ("args,a", value<std::string>(&args),
     "s-expression passed to interpreted code via platform:cmdline") 
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
      "phxi, the phxpr interpreter (Prana v" BSP_VERSION_STR ")\n"
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

//  try {
    std::ifstream ifs(input.c_str(), std::ifstream::in);  
    evaluator e;
 
    // basic environment 
    e.variables.define("begin", begin);
    e.variables.define("at", at);
    e.variables.define("<", lt);
    e.variables.define("<=", lte);
    e.variables.define("&&", and_);
    e.variables.define("||", or_);
    e.variables.define("if", if_);
    e.variables.define("list", list);
    e.variables.define("+", add);
    e.variables.define("-", min);
    e.variables.define("*", mul);
    e.variables.define("/", div_);
    e.variables.define("front", front);
    e.variables.define("back", back);
    e.variables.define("rest", rest);
    e.variables.define("!", not_);
    e.variables.define("empty?", not_);
    e.variables.define("push-front", push_front);
    e.variables.define("push-back", push_back);

    // temporary stand-ins for I/O, will be replaced by the ports subsystem
    e.variables.define("ports:display", display);
    e.variables.define("ports:newline", newline);
    
    // temporary stand-ins for the system interface, will be replaced by the
    // platform subsystem
    if (vm.count("args")) {
      // TODO: error handling
      parse_tree<sexpr> pt(args); 
      e.variables.define("platform:cmdline", procedure(quote(pt.ast())));
    }

    else
      e.variables.define("platform:cmdline", procedure(quote(utree(nil))));
 
    dynamic_array<boost::shared_ptr<parse_tree<sexpr> > > asts(16);
    utree r;

    // interpreter loop
    while (ifs.good()) {
      asts.push_back(boost::shared_ptr<parse_tree<sexpr> >());

      // read
      asts.back().reset(new parse_tree<sexpr>(ifs));

      // eval
      function f = e(asts.back()->ast());

      if (!ifs.good()) {
        if (!f.empty())
          r = f();
        break;
      }

      // invoke for side effects
      else if (!f.empty()) 
        f();
    }

    // print the result  
    std::cout << "return value: ";
    generate_sexpr(r, std::cout);
    std::cout << std::endl;

//  }

/*
  catch (std::exception& e) {
    std::cerr << "error: caught exception \"" << e.what() << "\"\n";
    return 1;
  }

  catch (...) {
    std::cerr << "error: caught unknown exception\n";
    return 1;
  }
*/

  return 0; 
}
 
