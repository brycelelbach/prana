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

  using boost::spirit::prana::phxpr::min;
  using boost::spirit::prana::phxpr::_1;
  using boost::spirit::prana::phxpr::_2;
  using boost::spirit::prana::phxpr::_3;

  // binary
  BOOST_TEST_EQ(min(2, 4)                   (),           utree(-2));
  BOOST_TEST_EQ(min(2, _1)                  (4),          utree(-2));
  BOOST_TEST_EQ(min(_1, _2)                 (2, 4),       utree(-2));
  BOOST_TEST_EQ(min(2, min(_1, _2))         (4, 8),       utree(6));
  
  // nary
  BOOST_TEST_EQ(min(2, 4, 8)                (),           utree(-10));
  BOOST_TEST_EQ(min(2, 4, _1)               (8),          utree(-10));
  BOOST_TEST_EQ(min(2, _1, _2)              (4, 8),       utree(-10));
  BOOST_TEST_EQ(min(2, 4, min(_1, _2, _3))  (8, 16, 32),  utree(38));

  // negative 
  BOOST_TEST_EQ(min(-2, 4)                  (),           utree(-6));
  BOOST_TEST_EQ(min(_1, 4)                  (-2),         utree(-6));
 
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}

