///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach 
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <phxpr/config.hpp>

#include <ios>
#include <iostream>
#include <sstream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <prana/version.hpp>
#include <prana/utree/io.hpp>

#include <sheol/adt/dynamic_array.hpp>

#include <prana/parse/parse_tree.hpp>
#include <prana/parse/grammar/sexpr.hpp>
#include <prana/generate/generate_sexpr.hpp>
    
#include <phxpr/version.hpp>
#include <phxpr/intrinsics/assert.hpp>
#include <phxpr/intrinsics/basic_arithmetic.hpp>
#include <phxpr/intrinsics/basic_io.hpp>
#include <phxpr/intrinsics/equivalence_predicates.hpp>
#include <phxpr/intrinsics/type_predicates.hpp>
#include <phxpr/evaluator.hpp>

using boost::program_options::variables_map;
using boost::program_options::options_description;
using boost::program_options::positional_options_description;
using boost::program_options::value;
using boost::program_options::store;
using boost::program_options::command_line_parser;
using boost::program_options::notify;

using boost::fusion::at_c;

using boost::spirit::nil;
using boost::spirit::utree;
using boost::spirit::utree_type;

using boost::filesystem::path;
using boost::filesystem::exists;
using boost::filesystem::is_directory;

using sheol::adt::dynamic_array;  

using prana::parse_tree;
using prana::tag::sexpr;
using prana::generate_sexpr;

using phxpr::evaluator;
using phxpr::evaluate;

using phxpr::addition;
using phxpr::subtraction;
using phxpr::multiplication;
using phxpr::division;

using phxpr::equal_predicate;

using phxpr::boolean_predicate;
using phxpr::symbol_predicate;
using phxpr::procedure_predicate;
using phxpr::list_predicate;
using phxpr::number_predicate;
using phxpr::string_predicate;
using phxpr::nil_predicate;
using phxpr::invalid_predicate;

using phxpr::display;
using phxpr::newline;

using phxpr::assertion;

int main (int argc, char** argv) {
  variables_map vm;

  options_description visible("Usage: phxi [options]"), hidden;
   
  std::string input(""), phex_input("");
 
  visible.add_options()
    ("help,h", "display this message")
    ("print-return", "print return value after evaluation")
    ("hide-pointers",
     "do not show C++ pointer values when displaying objects and procedures")
    ("phex,p", value<std::string>(&phex_input),
     "run in expectation mode, comparing evaluator stdout to expectation file")
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
      "phxi, the phxpr interpreter (v" PHXPR_VERSION_STR ")\n"
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

  else {
    path ip(input);

    if (!exists(ip)) {
      std::cerr << "error: phxpr input '" << input << "' does not exist\n";
      return 1;
    }

    else if (is_directory(ip)) {
      std::cerr << "error: phxpr input '" << input << "' is a directory\n";
      return 1;
    }
  }

  // phex variables
  boost::shared_ptr<std::ostringstream> phex_stdout;

  if (vm.count("phex")) {
    phex_stdout = boost::make_shared<std::ostringstream>();
    
    path ip(phex_input);

    if (!exists(ip)) {
      std::cerr << "error: phex expectation file '"
                << input << "' does not exist\n";
      return 1;
    }

    else if (is_directory(ip)) {
      std::cerr << "error: phex expectation file '"
                << input << "' is a directory\n";
      return 1;
    }
  }

  std::ifstream ifs(input.c_str(), std::ifstream::in);  
  evaluator e;

  const bool hide_pointers = vm.count("hide-pointers");

  // globals
  e.define_global("nil", nil);

  // basic arithmetic 
  e.define_intrinsic("+", addition());
  e.define_intrinsic("-", subtraction());
  e.define_intrinsic("*", multiplication());
  e.define_intrinsic("/", division());
 
  // equivalence predicates 
  e.define_intrinsic("=?", equal_predicate());

  // type predicates
  e.define_intrinsic("boolean?", boolean_predicate());
  e.define_intrinsic("symbol?", symbol_predicate());
  e.define_intrinsic("procedure?", procedure_predicate());
  e.define_intrinsic("list?", list_predicate());
  e.define_intrinsic("number?", number_predicate());
  e.define_intrinsic("string?", string_predicate());
  e.define_intrinsic("nil?", nil_predicate());
  e.define_intrinsic("unspecified?", invalid_predicate());

  // basic io
  if (vm.count("phex")) {
    e.define_intrinsic("display", display(*phex_stdout, !hide_pointers));
    e.define_intrinsic("newline", newline(*phex_stdout));
  }

  else {
    e.define_intrinsic("display", display(std::cout, !hide_pointers));
    e.define_intrinsic("newline", newline(std::cout));
  }
  
  // basic io
  e.define_intrinsic("assert", assertion());

  dynamic_array<boost::shared_ptr<parse_tree<sexpr> > > asts(16);

  if (vm.count("print-return")) {
    utree r;

    // interpreter file REL
    while (ifs.good()) {
      asts.push_back(boost::shared_ptr<parse_tree<sexpr> >());

      // read
      asts.back().reset(new parse_tree<sexpr>(ifs));

      // eval
      if (!ifs.good()) {
        r = evaluate(*asts.back(), e);
        break;
      }

      // invoke for side effects
      else 
        evaluate(*asts.back(), e);
    }

    if (vm.count("phex")) {
      (*phex_stdout) << "(return-value ";
      generate_sexpr(r, *phex_stdout, !hide_pointers);
      (*phex_stdout) << ")\n";
    }

    else {
      std::cout << "(return-value ";
      generate_sexpr(r, std::cout, !hide_pointers);
      std::cout << ")\n";
    }
  }

  else {
    // interpreter file REL
    while (ifs.good()) {
      asts.push_back(boost::shared_ptr<parse_tree<sexpr> >());

      // read
      asts.back().reset(new parse_tree<sexpr>(ifs));

      // eval
      if (!ifs.good()) {
        evaluate(*asts.back(), e);
        break;
      }

      // invoke for side effects
      else 
        evaluate(*asts.back(), e);
    }
  }

  if (vm.count("phex")) {
    std::ifstream phex_ifs(phex_input.c_str(), std::ifstream::in);  
    
    phex_ifs.unsetf(std::ios::skipws);

    std::size_t length = 0;
    char* buffer = 0;

    phex_ifs.seekg(0, std::ios::end);
    length = phex_ifs.tellg();
    phex_ifs.seekg(0, std::ios::beg);

    buffer = new char [length + std::size_t(1)];

    phex_ifs.read(buffer, length);
    phex_ifs.close();

    buffer[length] = '\0';

    std::string phex_str = phex_stdout->str();

    if (phex_str != buffer) {
      std::cerr << "error: evaluation output did not match phex expectation "
                << "file '" << phex_input << "'\n"
                <<   "=got=================================================\n"
                << phex_str
                << "\n=expected============================================\n"
                << buffer
                << "\n=====================================================\n";

      return 1;
    }

    delete[] buffer;
  }

  return 0; 
}
 
