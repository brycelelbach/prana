/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#include "harness.hpp"

#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/sexpr/core.hpp>
#include <boost/spirit/home/prana/sexpr/routine/become.hpp>

int main (void) {
  using namespace boost::spirit::prana;

  { //[boolean_to_boolean
    sexpr s(true);

    become<sexpr::boolean>(s); 

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("#false")))
    //]
  }
  
  { //[floating_to_integer
    sexpr s(21.53);

    become<sexpr::integer>(s);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("0")))
    //]
  }
  
  { //[integer_to_ascii
    sexpr s(14);

    become<sexpr::ascii>(s);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("\"\"")))
    //]
  }

  return boost::report_errors();
}
 
