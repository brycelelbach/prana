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
  using boost::spirit::prana::phxpr::_1;
  using boost::spirit::prana::phxpr::_2;
  using boost::spirit::prana::phxpr::_3;

  {
    utree ut;

    ut.push_back(1);
    ut.push_back(2);
    ut.push_back(3);  

    BOOST_TEST_EQ(list(1, 2, 3)   (),     ut);
    BOOST_TEST_EQ(list(1, 2, _1)  (3),    ut);
    BOOST_TEST_EQ(list(1, _1, _2) (2, 3), ut);
  }
  
  {
    utree ut1, ut2;

    ut2.push_back(1);
    ut1.push_back(ut2);
    ut2.clear();
    
    ut2.push_back(2);
    ut1.push_back(ut2);
    ut2.clear();
    
    ut2.push_back(3);
    ut1.push_back(ut2);

    BOOST_TEST_EQ(list(list(1), list(2), list(3))   (),       ut1);
    BOOST_TEST_EQ(list(list(1), list(_1), _2)       (2, ut2), ut1);
  }
 
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}

