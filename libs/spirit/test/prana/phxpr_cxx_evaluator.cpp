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

  using boost::spirit::prana::parse_tree;
  using boost::spirit::prana::tag::sexpr;
  using boost::spirit::prana::phxpr::evaluator;
  using boost::spirit::prana::phxpr::function;
  using boost::spirit::prana::phxpr::procedure;
  using boost::spirit::prana::phxpr::lambda;
  using boost::spirit::prana::phxpr::if_;
  using boost::spirit::prana::phxpr::lte;
  using boost::spirit::prana::phxpr::lt;
  using boost::spirit::prana::phxpr::mul;
  using boost::spirit::prana::phxpr::min;
  using boost::spirit::prana::phxpr::add;
  using boost::spirit::prana::phxpr::_1;
  using boost::spirit::prana::phxpr::_this;

  /// phxpr        
  /// (define fact
  ///   (lambda (_1)
  ///     (if (<= _1 0) 1
  ///       (* _1 (fact (- _1 1))))))

  /// C++ 
      function fact =
        lambda (_1)
          (if_ (lte (_1, 0), 1,
            mul (_1, _this (min (_1, 1)))));

  evaluator e;

  e.functions.define("factorial", procedure(fact));

  parse_tree<sexpr> function_call("(factorial 10)"); 

  utree r = e(function_call.ast())(); 

  BOOST_TEST_EQ(r, utree(3628800));

  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}

