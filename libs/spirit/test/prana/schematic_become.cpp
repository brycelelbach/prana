/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/schematic/core.hpp>
#include <boost/spirit/home/prana/schematic/routine/become.hpp>

int main (void) {
  using namespace boost::spirit::prana;

  { //[boolean_to_boolean
    schematic s(true);

    become<schematic::boolean>(s); 

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("#f")))
    //]
  }
  
  { //[floating_to_integer
    schematic s(21.53);

    become<schematic::integer>(s);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("0")))
    //]
  }
  
  { //[integer_to_ascii
    schematic s(14);

    become<schematic::ascii>(s);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("\"\"")))
    //]
  }

  return boost::report_errors();
}
 
