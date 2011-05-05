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

using boost::spirit::utree;
using boost::spirit::utf8_symbol_type;

using boost::spirit::prana::parse_tree;
using boost::spirit::prana::tag::sexpr;
using boost::spirit::prana::generate_sexpr;
using boost::spirit::prana::phxpr::pattern;
using boost::spirit::prana::phxpr::matcher;

namespace {

struct pattern_test {
  pattern::literals_type lits;
  parse_tree<sexpr> elements;
  parse_tree<sexpr> body;
  boost::shared_ptr<pattern> p;

  pattern_test& patt (char const* elements_, char const* body_) {
    lits.clear();
    elements = elements_;
    body = body_;
    p.reset(new pattern(lits, elements.ast(), body.ast()));
    return *this;
  }

  pattern_test& use_pass (char const* use_, char const* expansion) {
    BOOST_TEST(p);

    parse_tree<sexpr> use(use_);
    boost::shared_ptr<matcher> m = p->match(use.ast());

    BOOST_TEST(m);

    if (m) {
      boost::shared_ptr<utree> e = m->expand();

      BOOST_TEST(e);      

      if (e) {
        BSP_STRINGIFY_TESTS(
          generate_sexpr,
          ((*e) (expansion)))
      }
    }
    return *this;
  }
  
  pattern_test& use_fail (char const* use_) {
    BOOST_TEST(p);

    parse_tree<sexpr> use(use_);
    boost::shared_ptr<matcher> m = p->match(use.ast());

    BOOST_TEST(!m);

    return *this;
  }
};    

}

int main (void) {
  pattern_test test;

  test
    .patt("(a b c)",
          "((* a b) (* b c))")
      .use_pass("(1 2 3)",
                "((* 1 2) (* 2 3))")
      .use_pass("(1 2 x)",
                "((* 1 2) (* 2 x))")

    .patt("(x y ...)",
          "(for-each (lambda (i) (set! x (+ x i))) y)")
      .use_pass("(foo 2 4 6 8)",
                "(for-each (lambda (i) (set! foo (+ foo i))) (2 4 6 8))")
      .use_pass("(bar)",
                "(for-each (lambda (i) (set! bar (+ bar i))) ())")
  ;

  return boost::report_errors();
}

