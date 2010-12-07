/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#include <iostream>

#include <boost/detail/lightweight_test.hpp>

#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/sexpr/core.hpp>
#include <boost/spirit/home/prana/sexpr/routine/capacity.hpp>

int main (void) {
  using namespace boost;
  using namespace boost::spirit;

  using prana::sexpr;

  { //[capacity_2_ascii
    sexpr s("f");
    
    BOOST_TEST(prana::capacity(s) == 2);
    //]
  }
  
  { //[capacity_4_ascii
    sexpr s("bar");
    
    BOOST_TEST(prana::capacity(s) == 4);
    //]
  }
  
  { //[capacity_16_ascii
    sexpr s("fizzbuzz");
    
    BOOST_TEST(prana::capacity(s) == 16);
    //]
  }

  return boost::report_errors();
}
 
