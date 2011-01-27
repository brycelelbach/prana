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

  using boost::spirit::prana::phxpr::eq;
  using boost::spirit::prana::phxpr::_1;
  using boost::spirit::prana::phxpr::_2;
  using boost::spirit::prana::phxpr::_3;
  
  // binary
  BOOST_TEST_EQ(eq(1, 1)                  (),         utree(true));
  BOOST_TEST_EQ(eq(1, _1)                 (0),        utree(false));
  BOOST_TEST_EQ(eq(_1, _2)                (0, 0),     utree(true));
  BOOST_TEST_EQ(eq(1, eq(_1, _2))         (0, 0),     utree(false));

  // nary
  BOOST_TEST_EQ(eq(1, 1, 0)               (),         utree(false));
  BOOST_TEST_EQ(eq(1, 1, _1)              (1),        utree(true));
  BOOST_TEST_EQ(eq(0, _1, _2)             (0, 1),     utree(false));
  BOOST_TEST_EQ(eq(1, 1, eq(_1, _2, _3))  (1, 1, 1),  utree(true));
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}

