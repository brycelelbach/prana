/*==============================================================================
    Copyright (c) 2010-2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/phxpr/pattern.hpp>

int main (void) { //try {
  using boost::spirit::utree;
  using boost::spirit::utf8_symbol_type;

  using boost::spirit::prana::phxpr::pattern;
  using boost::spirit::prana::phxpr::matcher;

  std::cout << "basic test" << std::endl; 

  { // basic 
    pattern::literals_type lits;

    utree elements;
    elements.push_back(utf8_symbol_type("a"));
    elements.push_back(utf8_symbol_type("b"));
    elements.push_back(utf8_symbol_type("c"));

    pattern p(lits, elements, utree());

    utree use;
    use.push_back(1);
    use.push_back(2.4);
    use.push_back("f");

    boost::shared_ptr<matcher> m = p.match(use);

    BOOST_TEST(m);

    if (m) {
      std::cout << "  bindings:" << std::endl;
      BOOST_FOREACH(matcher::bindings_type::const_reference kv, m->bindings()) {
        std::cout << "    " << kv.first << " -> " << kv.second << std::endl;
      }
    }
  }
  
  std::cout << std::endl << "no match (greater arity) test" << std::endl; 
  
  { // no match (greater arity)
    pattern::literals_type lits;

    utree elements;
    elements.push_back(utf8_symbol_type("a"));
    elements.push_back(utf8_symbol_type("b"));
    elements.push_back(utf8_symbol_type("c"));

    pattern p(lits, elements, utree());

    utree use;
    use.push_back(1);
    use.push_back(2.4);
    use.push_back("f");
    use.push_back(true);

    boost::shared_ptr<matcher> m = p.match(use);

    BOOST_TEST(!m);
  }
  
  std::cout << std::endl << "no match (lesser arity) test" << std::endl; 
  
  { // no match (lesser arity)
    pattern::literals_type lits;

    utree elements;
    elements.push_back(utf8_symbol_type("a"));
    elements.push_back(utf8_symbol_type("b"));
    elements.push_back(utf8_symbol_type("c"));

    pattern p(lits, elements, utree());

    utree use;
    use.push_back(1);
    use.push_back(2.4);

    boost::shared_ptr<matcher> m = p.match(use);

    BOOST_TEST(!m);
  }
  
  std::cout << std::endl << "variable argument test" << std::endl; 
  
  { // variable argument 
    pattern::literals_type lits;

    utree elements;
    elements.push_back(utf8_symbol_type("a"));
    elements.push_back(utf8_symbol_type("b"));
    elements.push_back(utf8_symbol_type("c"));
    elements.push_back(utf8_symbol_type("..."));

    pattern p(lits, elements, utree());

    utree use;
    use.push_back(1);
    use.push_back(2.4);
    use.push_back("f");

    boost::shared_ptr<matcher> m = p.match(use);

    BOOST_TEST(m);

    if (m) {
      std::cout << "  bindings:" << std::endl;
      BOOST_FOREACH(matcher::bindings_type::const_reference kv, m->bindings()) {
        std::cout << "    " << kv.first << " -> " << kv.second << std::endl;
      }
    }
  }

//  } catch (std::exception& e) {
//    std::cout << "caught: " << e.what() << "\n";
//    return -1;
//  }

  return boost::report_errors();
}

