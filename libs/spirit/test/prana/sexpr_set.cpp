/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#include "harness.hpp"

#include <boost/spirit/home/prana/sexpr/core.hpp>
#include <boost/spirit/home/prana/sexpr/routine/set.hpp>

int main (void) {
  using namespace boost::spirit::prana;

  { //[string_literal
    sexpr s;
    
    set(s, "abc");

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("\"abc\"")))
    //]
  }
  
  { //[integer_literal
    sexpr s;
    
    set(s, 5);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("5")))
    //]
  }
  
  return boost::report_errors();
}
 
