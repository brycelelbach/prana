/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#include "harness.hpp"

#include <boost/spirit/home/prana/sexpr/core.hpp>
#include <boost/spirit/home/prana/sexpr/routine/set_car.hpp>

int main (void) {
  using namespace boost::spirit::prana;

  { 
    sexpr s(true, false);
    
    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("(#true #false)")))

    set_car(s, 4);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("(4 #false)")))
  }
  
  { 
    sexpr s("pingpong", 312.12);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("(\"pingpong\" 312.12)")))

    set_car(s, "fizz");

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("(\"fizz\" 312.12)")))
  }
  
  return boost::report_errors();
}
 
