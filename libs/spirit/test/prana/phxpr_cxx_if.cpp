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

  using boost::spirit::prana::phxpr::if_;
  using boost::spirit::prana::phxpr::_1;
  using boost::spirit::prana::phxpr::_2;
  using boost::spirit::prana::phxpr::_3;

  // if branch
  BOOST_TEST_EQ(if_(0, 1, 2)                  (),         utree(2));
  BOOST_TEST_EQ(if_(0, 1, _1)                 (2),        utree(2));
  BOOST_TEST_EQ(if_(0, _1, _2)                (1, 2),     utree(2));
  BOOST_TEST_EQ(if_(0, _1, if_(_2, _3, _1))   (1, 2, 3),  utree(3));
  
  // else branch
  BOOST_TEST_EQ(if_(3, 4, 5)                  (),         utree(4));
  BOOST_TEST_EQ(if_(3, 4, _1)                 (5),        utree(4));
  BOOST_TEST_EQ(if_(3, _1, _2)                (4, 5),     utree(4));
  BOOST_TEST_EQ(if_(3, _1, if_(_2, _3, _1))   (4, 5, 6),  utree(4));
 
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}

