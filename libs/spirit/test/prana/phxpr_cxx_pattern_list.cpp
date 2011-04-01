/*==============================================================================
    Copyright (c) 2010-2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/parse/parse_tree.hpp>
#include <boost/spirit/home/prana/parse/grammar/sexpr.hpp>
#include <boost/spirit/home/prana/generate/generate_sexpr.hpp>
#include <boost/spirit/home/prana/phxpr/pattern.hpp>

int main (void) { try {
  using boost::spirit::utree;
  using boost::spirit::utf8_symbol_type;
  
  using boost::spirit::prana::parse_tree;
  using boost::spirit::prana::tag::sexpr;
  using boost::spirit::prana::generate_sexpr;
  using boost::spirit::prana::phxpr::pattern;
  using boost::spirit::prana::phxpr::matcher;

  std::cout << "(list-pattern-test" << std::endl; 

  { 
    pattern::literals_type lits;

    parse_tree<sexpr> elements("((a b) c)");
    
    std::cout << "  (pattern ";
    generate_sexpr(elements.ast(), std::cout);
    std::cout << ")" << std::endl; 

    pattern p(lits, elements.ast(), utree());

    parse_tree<sexpr> use("((1 2.4) \"f\")");

    std::cout << "  (use ";
    generate_sexpr(use.ast(), std::cout);
    std::cout << ")" << std::endl; 

    boost::shared_ptr<matcher> m = p.match(use.ast());

    BOOST_TEST(m);

    if (m) {
      std::cout << "  (bindings";
      BOOST_FOREACH(matcher::bindings_type::const_reference kv, m->bindings()) {
        std::cout << "\n    (";
        generate_sexpr(kv.first, std::cout);
        std::cout << " ";
        generate_sexpr(kv.second, std::cout);
        std::cout << ")";
      }
      std::cout << ")" << std::endl;
    }
  }
  
  std::cout << "  )\n" << std::endl;

  { // no match (greater arity)
    pattern::literals_type lits;

    parse_tree<sexpr> elements("((a b) c)");

    pattern p(lits, elements.ast(), utree());

    parse_tree<sexpr> use("((1 2.4) \"f\" #t)");

    boost::shared_ptr<matcher> m = p.match(use.ast());

    BOOST_TEST(!m);
  }
  
  { // no match (lesser arity)
    pattern::literals_type lits;

    parse_tree<sexpr> elements("((a b) c)");

    pattern p(lits, elements.ast(), utree());

    parse_tree<sexpr> use("((1 2.4))");

    boost::shared_ptr<matcher> m = p.match(use.ast());

    BOOST_TEST(!m);
  }
  
  std::cout << "(variadic-pattern-with-list-test" << std::endl; 
  
  {
    pattern::literals_type lits;

    parse_tree<sexpr> elements("((a b) c ...)");

    std::cout << "  (pattern ";
    generate_sexpr(elements.ast(), std::cout);
    std::cout << ")" << std::endl; 

    pattern p(lits, elements.ast(), utree());

    parse_tree<sexpr> use("((1 2.4) \"f\")");

    std::cout << "  (use ";
    generate_sexpr(use.ast(), std::cout);
    std::cout << ")" << std::endl; 

    boost::shared_ptr<matcher> m = p.match(use.ast());

    BOOST_TEST(m);

    if (m) {
      std::cout << "  (bindings";
      BOOST_FOREACH(matcher::bindings_type::const_reference kv, m->bindings()) {
        std::cout << "\n    (";
        generate_sexpr(kv.first, std::cout);
        std::cout << " ";
        generate_sexpr(kv.second, std::cout);
        std::cout << ")";
      }
      std::cout << ")" << std::endl;
    }
  }
  
  std::cout << "  )\n\n(variadic-pattern-with-list-greater-arity-test" << std::endl; 
  
  { 
    pattern::literals_type lits;

    parse_tree<sexpr> elements("((a b) c ...)");

    std::cout << "  (pattern ";
    generate_sexpr(elements.ast(), std::cout);
    std::cout << ")" << std::endl; 

    pattern p(lits, elements.ast(), utree());

    parse_tree<sexpr> use("((1 2.4) \"f\" #t)");

    std::cout << "  (use ";
    generate_sexpr(use.ast(), std::cout);
    std::cout << ")" << std::endl; 

    boost::shared_ptr<matcher> m = p.match(use.ast());

    BOOST_TEST(m);

    if (m) {
      std::cout << "  (bindings";
      BOOST_FOREACH(matcher::bindings_type::const_reference kv, m->bindings()) {
        std::cout << "\n    (";
        generate_sexpr(kv.first, std::cout);
        std::cout << " ";
        generate_sexpr(kv.second, std::cout);
        std::cout << ")";
      }
      std::cout << ")" << std::endl;
    }
  }

  std::cout << "  )\n\n(variadic-pattern-with-list-lesser-arity-test" << std::endl; 
  
  { 
    pattern::literals_type lits;

    parse_tree<sexpr> elements("((a b) c ...)");

    std::cout << "  (pattern ";
    generate_sexpr(elements.ast(), std::cout);
    std::cout << ")" << std::endl; 

    pattern p(lits, elements.ast(), utree());

    parse_tree<sexpr> use("((1 2.4))");

    std::cout << "  (use ";
    generate_sexpr(use.ast(), std::cout);
    std::cout << ")" << std::endl; 

    boost::shared_ptr<matcher> m = p.match(use.ast());

    BOOST_TEST(m);

    if (m) {
      std::cout << "  (bindings";
      BOOST_FOREACH(matcher::bindings_type::const_reference kv, m->bindings()) {
        std::cout << "\n    (";
        generate_sexpr(kv.first, std::cout);
        std::cout << " ";
        generate_sexpr(kv.second, std::cout);
        std::cout << ")";
      }
      std::cout << ")" << std::endl;
    }
  }

  std::cout << "  )\n\n(variadic-list-pattern-test" << std::endl; 
  
  { 
    pattern::literals_type lits;

    parse_tree<sexpr> elements("((a b) ...)");

    std::cout << "  (pattern ";
    generate_sexpr(elements.ast(), std::cout);
    std::cout << ")" << std::endl; 

    pattern p(lits, elements.ast(), utree());

    parse_tree<sexpr> use("((1 2.4))");

    std::cout << "  (use ";
    generate_sexpr(use.ast(), std::cout);
    std::cout << ")" << std::endl; 

    boost::shared_ptr<matcher> m = p.match(use.ast());

    BOOST_TEST(m);

    if (m) {
      std::cout << "  (bindings";
      BOOST_FOREACH(matcher::bindings_type::const_reference kv, m->bindings()) {
        std::cout << "\n    (";
        generate_sexpr(kv.first, std::cout);
        std::cout << " ";
        generate_sexpr(kv.second, std::cout);
        std::cout << ")";
      }
      std::cout << ")" << std::endl;
    }
  }

  std::cout << "  )\n\n(variadic-list-pattern-greater-arity-test" << std::endl; 
  
  { 
    pattern::literals_type lits;

    parse_tree<sexpr> elements("((a b) ...)");

    std::cout << "  (pattern ";
    generate_sexpr(elements.ast(), std::cout);
    std::cout << ")" << std::endl; 

    pattern p(lits, elements.ast(), utree());

    parse_tree<sexpr> use("((foo 1) (bar 2.4))");

    std::cout << "  (use ";
    generate_sexpr(use.ast(), std::cout);
    std::cout << ")" << std::endl; 

    boost::shared_ptr<matcher> m = p.match(use.ast());

    BOOST_TEST(m);

    if (m) {
      std::cout << "  (bindings";
      BOOST_FOREACH(matcher::bindings_type::const_reference kv, m->bindings()) {
        std::cout << "\n    (";
        generate_sexpr(kv.first, std::cout);
        std::cout << " ";
        generate_sexpr(kv.second, std::cout);
        std::cout << ")";
      }
      std::cout << ")" << std::endl;
    }
  }

  std::cout << "  )\n\n(r5rs-scheme-let-macro-test" << std::endl; 
  
  { 
    pattern::literals_type lits;

    parse_tree<sexpr> elements("(((name val) ...) body1 body2 ...)");

    std::cout << "  (pattern ";
    generate_sexpr(elements.ast(), std::cout);
    std::cout << ")" << std::endl; 

    pattern p(lits, elements.ast(), utree());

    parse_tree<sexpr> use("(((x 2) (y 3)) (* x y))");

    std::cout << "  (use ";
    generate_sexpr(use.ast(), std::cout);
    std::cout << ")" << std::endl; 

    boost::shared_ptr<matcher> m = p.match(use.ast());

    BOOST_TEST(m);

    if (m) {
      std::cout << "  (bindings";
      BOOST_FOREACH(matcher::bindings_type::const_reference kv, m->bindings()) {
        std::cout << "\n    (";
        generate_sexpr(kv.first, std::cout);
        std::cout << " ";
        generate_sexpr(kv.second, std::cout);
        std::cout << ")";
      }
      std::cout << ")" << std::endl;
    }
  }

  std::cout << "  )" << std::endl;

  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}

