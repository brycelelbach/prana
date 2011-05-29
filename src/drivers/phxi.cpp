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
#include <boost/fusion/include/at_c.hpp>

#include <prana/version.hpp>
#include <prana/utree/io.hpp>

#include <sheol/adt/dynamic_array.hpp>

#include <prana/parse/parse_tree.hpp>
#include <prana/parse/grammar/sexpr.hpp>
#include <prana/generate/generate_sexpr.hpp>
    
#include <phxpr/version.hpp>
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

using sheol::adt::dynamic_array;  

using prana::parse_tree;
using prana::tag::sexpr;
using prana::generate_sexpr;

using phxpr::signature;
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
  e.define_intrinsic("display", display(std::cout));
  e.define_intrinsic("newline", newline(std::cout));

  dynamic_array<boost::shared_ptr<parse_tree<sexpr> > > asts(16);
  utree r;

  // interpreter REL
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

  // print the result  
  std::cout << "return value: ";
  generate_sexpr(r, std::cout);
  std::cout << std::endl;

  std::cout << "return value tag: " << r.tag() << std::endl;

  if (e.global_procedure_table->size() > std::size_t(r.tag())) {
    signature const& sig = (*e.global_procedure_table)[r.tag()];
    std::cout << "displacement: " << at_c<0>(sig) << std::endl;
    std::cout << "arity type: " << at_c<1>(sig) << std::endl;
    std::cout << "evaluation strategy: " << at_c<2>(sig) << std::endl;
    std::cout << "function type: " << at_c<3>(sig) << std::endl;
  }
  
  else
    std::cout << "tag is not in the gpt" << std::endl;

  return 0; 
}
 
