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

  using boost::spirit::prana::phxpr::not_;
  using boost::spirit::prana::phxpr::_1;
  using boost::spirit::prana::phxpr::_2;
  using boost::spirit::prana::phxpr::_3;

  // basic
  BOOST_TEST_EQ(not_(true)        (),      utree(false));
  BOOST_TEST_EQ(not_(false)       (),      utree(true));
  BOOST_TEST_EQ(not_(_1)          (true),  utree(false));
  BOOST_TEST_EQ(not_(_1)          (false), utree(true));
  
  // nested
  BOOST_TEST_EQ(not_(not_(true))  (),      utree(true));
  BOOST_TEST_EQ(not_(not_(false)) (),      utree(false));
  BOOST_TEST_EQ(not_(not_(_1))    (true),  utree(true));
  BOOST_TEST_EQ(not_(not_(_1))    (false), utree(false));
 
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}

