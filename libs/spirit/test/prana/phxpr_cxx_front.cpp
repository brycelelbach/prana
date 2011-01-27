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

  using boost::spirit::prana::phxpr::list;
  using boost::spirit::prana::phxpr::front;
  using boost::spirit::prana::phxpr::_1;
  using boost::spirit::prana::phxpr::_2;
  using boost::spirit::prana::phxpr::_3;

  utree ut;

  ut.push_back(1);
  ut.push_back(2);
  ut.push_back(3);  

  // basic
  BOOST_TEST_EQ(front(list(1, 2, 3))                    (),     1);
  BOOST_TEST_EQ(front(list(1, _1, _2))                  (2, 3), 1);
  BOOST_TEST_EQ(front(_1)                               (ut),   1);

  // nested
  BOOST_TEST_EQ(front(front(list(list(1, 2, 3), 0)))    (),     1);
  BOOST_TEST_EQ(front(front(list(list(1, _1, _2), 0)))  (2, 3), 1);
  BOOST_TEST_EQ(front(front(list(_1, 0)))               (ut),   1);

 
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}

