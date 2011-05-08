/*==============================================================================
    Copyright (c) 2010-2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/parse/parse_tree.hpp>
#include <boost/spirit/home/prana/parse/grammar/sexpr.hpp>
#include <boost/spirit/home/prana/phxpr/evaluator.hpp>
#include <boost/spirit/home/prana/phxpr/core.hpp>
#include <boost/spirit/home/prana/phxpr/intrinsics.hpp>

int main (void) { try {
  using boost::spirit::utree;
  using boost::spirit::scope;

  using boost::spirit::prana::parse_tree;
  using boost::spirit::prana::tag::sexpr;
  using boost::spirit::prana::phxpr::evaluator;
  using boost::spirit::prana::phxpr::function;
  using boost::spirit::prana::phxpr::procedure;
  using boost::spirit::prana::phxpr::lambda;
  using boost::spirit::prana::phxpr::if_;
  using boost::spirit::prana::phxpr::lte;
  using boost::spirit::prana::phxpr::mul;
  using boost::spirit::prana::phxpr::min;
  using boost::spirit::prana::phxpr::_1;
  using boost::spirit::prana::phxpr::_this;

  {
    /// phxpr        
    parse_tree<sexpr> times2_definition(
      "(define-variable times-2\n"
      "   (lambda (_1)\n"
      "     (* 2 _1)))");
  
    evaluator e;
  
    e.variables->define("*", mul);
  
    e(times2_definition.ast());
 
    parse_tree<sexpr> times2_call("(times-2 1)"); 
    utree r = e(times2_call.ast())(); 
    BOOST_TEST_EQ(r, utree(2));
    
    times2_call = "(times-2 2)"; 
    r = e(times2_call.ast())(); 
    BOOST_TEST_EQ(r, utree(4));

    times2_call = "(times-2 3)"; 
    r = e(times2_call.ast())(); 
    BOOST_TEST_EQ(r, utree(6));
  }

  {
    /// phxpr        
    parse_tree<sexpr> fact_definition(
      "(define-variable fact\n"
      "   (lambda (_1)\n"
      "     (if (<= _1 0) 1\n"
      "       (* _1 (fact (- _1 1))))))");
  
    evaluator e;
  
    e.variables->define("if", if_);
    e.variables->define("<=", lte);
    e.variables->define("*", mul);
    e.variables->define("-", min);
  
    e(fact_definition.ast());
 
    parse_tree<sexpr> fact_call("(fact 0)"); 
    utree r = e(fact_call.ast())(); 
    BOOST_TEST_EQ(r, utree(1));
    
    fact_call = "(fact 1)"; 
    r = e(fact_call.ast())(); 
    BOOST_TEST_EQ(r, utree(1));

    fact_call = "(fact 2)"; 
    r = e(fact_call.ast())(); 
    BOOST_TEST_EQ(r, utree(2));
    
    fact_call = "(fact 3)"; 
    r = e(fact_call.ast())(); 
    BOOST_TEST_EQ(r, utree(6));
    
    fact_call = "(fact 4)"; 
    r = e(fact_call.ast())(); 
    BOOST_TEST_EQ(r, utree(24));
    
    fact_call = "(fact 5)"; 
    r = e(fact_call.ast())(); 
    BOOST_TEST_EQ(r, utree(120));
    
    fact_call = "(fact 6)"; 
    r = e(fact_call.ast())(); 
    BOOST_TEST_EQ(r, utree(720));
    
    fact_call = "(fact 7)"; 
    r = e(fact_call.ast())(); 
    BOOST_TEST_EQ(r, utree(5040));
    
    fact_call = "(fact 8)"; 
    r = e(fact_call.ast())(); 
    BOOST_TEST_EQ(r, utree(40320));
    
    fact_call = "(fact 9)"; 
    r = e(fact_call.ast())(); 
    BOOST_TEST_EQ(r, utree(362880));
    
    fact_call = "(fact 10)"; 
    r = e(fact_call.ast())(); 
    BOOST_TEST_EQ(r, utree(3628800));
  }

  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}

