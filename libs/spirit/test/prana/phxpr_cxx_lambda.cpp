/*==============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2010-2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/phxpr/core.hpp>
#include <boost/spirit/home/prana/phxpr/intrinsics.hpp>

int main (void) { try {
  using boost::spirit::utree;

  using boost::spirit::prana::phxpr::function;
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

  BOOST_TEST_EQ(fact(0),  utree(1));
  BOOST_TEST_EQ(fact(1),  utree(1));
  BOOST_TEST_EQ(fact(2),  utree(2));
  BOOST_TEST_EQ(fact(3),  utree(6));
  BOOST_TEST_EQ(fact(4),  utree(24));
  BOOST_TEST_EQ(fact(5),  utree(120));
  BOOST_TEST_EQ(fact(6),  utree(720));
  BOOST_TEST_EQ(fact(7),  utree(5040));
  BOOST_TEST_EQ(fact(8),  utree(40320));
  BOOST_TEST_EQ(fact(9),  utree(362880));
  BOOST_TEST_EQ(fact(10), utree(3628800));

  BOOST_TEST_EQ(
    (lambda (_1) (if_ (lte (_1, 0), 1, mul (_1, _this (min (_1, 1)))))) (0),
      utree(1));
  
  BOOST_TEST_EQ(
    (lambda (_1) (if_ (lte (_1, 0), 1, mul (_1, _this (min (_1, 1)))))) (1),
      utree(1));
  
  BOOST_TEST_EQ(
    (lambda (_1) (if_ (lte (_1, 0), 1, mul (_1, _this (min (_1, 1)))))) (2),
      utree(2));
  
  BOOST_TEST_EQ(
    (lambda (_1) (if_ (lte (_1, 0), 1, mul (_1, _this (min (_1, 1)))))) (3),
      utree(6));
  
  BOOST_TEST_EQ(
    (lambda (_1) (if_ (lte (_1, 0), 1, mul (_1, _this (min (_1, 1)))))) (4),
      utree(24));

  BOOST_TEST_EQ(
    (lambda (_1) (if_ (lte (_1, 0), 1, mul (_1, _this (min (_1, 1)))))) (5),
      utree(120));

  BOOST_TEST_EQ(
    (lambda (_1) (if_ (lte (_1, 0), 1, mul (_1, _this (min (_1, 1)))))) (6),
      utree(720));
  
  BOOST_TEST_EQ(
    (lambda (_1) (if_ (lte (_1, 0), 1, mul (_1, _this (min (_1, 1)))))) (7),
      utree(5040));
  
  BOOST_TEST_EQ(
    (lambda (_1) (if_ (lte (_1, 0), 1, mul (_1, _this (min (_1, 1)))))) (8),
      utree(40320));
  
  BOOST_TEST_EQ(
    (lambda (_1) (if_ (lte (_1, 0), 1, mul (_1, _this (min (_1, 1)))))) (9),
      utree(362880));

  BOOST_TEST_EQ(
    (lambda (_1) (if_ (lte (_1, 0), 1, mul (_1, _this (min (_1, 1)))))) (10),
      utree(3628800));

  /////////////////////////////////////////////////////////////////////////////
  /// fibonacci sequence
  /////////////////////////////////////////////////////////////////////////////

  /// phxpr
  /// (define fib
  ///   (lambda (_1)
  ///     (if (< _1 2) _1
  ///       (+ (fib (- _1 1)) (fib (- _1 2)))))) 

  /// C++
      function fib =
        lambda (_1)
          (if_ (lt (_1, 2), _1,
            add (_this (min (_1, 1)), _this (min (_1, 2)))));
  
  BOOST_TEST_EQ(fib(0),  utree(0));
  BOOST_TEST_EQ(fib(1),  utree(1));
  BOOST_TEST_EQ(fib(2),  utree(1));
  BOOST_TEST_EQ(fib(3),  utree(2));
  BOOST_TEST_EQ(fib(4),  utree(3));
  BOOST_TEST_EQ(fib(5),  utree(5));
  BOOST_TEST_EQ(fib(6),  utree(8));
  BOOST_TEST_EQ(fib(7),  utree(13));
  BOOST_TEST_EQ(fib(8),  utree(21));
  BOOST_TEST_EQ(fib(9),  utree(34));
  BOOST_TEST_EQ(fib(10), utree(55));
    
  BOOST_TEST_EQ(    
    (lambda (_1) (if_ (lt (_1, 2), _1,
      add (_this (min (_1, 1)), _this (min (_1, 2)))))) (0),
        utree(0));
  
  BOOST_TEST_EQ(    
    (lambda (_1) (if_ (lt (_1, 2), _1,
      add (_this (min (_1, 1)), _this (min (_1, 2)))))) (1),
        utree(1));
    
  BOOST_TEST_EQ(    
    (lambda (_1) (if_ (lt (_1, 2), _1,
      add (_this (min (_1, 1)), _this (min (_1, 2)))))) (2),
        utree(1));
  
  BOOST_TEST_EQ(    
    (lambda (_1) (if_ (lt (_1, 2), _1,
      add (_this (min (_1, 1)), _this (min (_1, 2)))))) (3),
        utree(2));
    
  BOOST_TEST_EQ(    
    (lambda (_1) (if_ (lt (_1, 2), _1,
      add (_this (min (_1, 1)), _this (min (_1, 2)))))) (4),
        utree(3));
    
  BOOST_TEST_EQ(    
    (lambda (_1) (if_ (lt (_1, 2), _1,
      add (_this (min (_1, 1)), _this (min (_1, 2)))))) (5),
        utree(5));
  
  BOOST_TEST_EQ(    
    (lambda (_1) (if_ (lt (_1, 2), _1,
      add (_this (min (_1, 1)), _this (min (_1, 2)))))) (6),
        utree(8));
    
  BOOST_TEST_EQ(    
    (lambda (_1) (if_ (lt (_1, 2), _1,
      add (_this (min (_1, 1)), _this (min (_1, 2)))))) (7),
        utree(13));
  
  BOOST_TEST_EQ(    
    (lambda (_1) (if_ (lt (_1, 2), _1,
      add (_this (min (_1, 1)), _this (min (_1, 2)))))) (8),
        utree(21));
    
  BOOST_TEST_EQ(    
    (lambda (_1) (if_ (lt (_1, 2), _1,
      add (_this (min (_1, 1)), _this (min (_1, 2)))))) (9),
        utree(34));
  
  BOOST_TEST_EQ(    
    (lambda (_1) (if_ (lt (_1, 2), _1,
      add (_this (min (_1, 1)), _this (min (_1, 2)))))) (10),
        utree(55));

  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}

