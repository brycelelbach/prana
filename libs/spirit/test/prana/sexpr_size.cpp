/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#include <iostream>

#include <boost/detail/lightweight_test.hpp>

#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/sexpr/core.hpp>
#include <boost/spirit/home/prana/sexpr/routine/size.hpp>

int main (void) {
  using namespace boost;
  using namespace boost::spirit;

  using prana::sexpr;

  { //[size_0_ascii
    sexpr s("");
    
    BOOST_TEST(prana::size(s) == 0);
    //]
  }
  
  { //[size_3_ascii
    sexpr s("bar");
    
    BOOST_TEST(prana::size(s) == 3);
    //]
  }
  
  { //[size_8_ascii
    sexpr s("fizzbuzz");
    
    BOOST_TEST(prana::size(s) == 8);
    //]
  }

  return boost::report_errors();
}
 
