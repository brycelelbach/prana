/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>
#include <boost/spirit/home/prana/schematic/routine/clear.hpp>
#include <boost/spirit/home/prana/schematic/routine/set.hpp>

int main (void) {
  using namespace boost::spirit::prana;

  { //[boolean
    schematic s;

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("nil")))

    set(s, true);
    clear(s);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("nil")))
    //]
  }

  { //[ascii
    schematic s;

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("nil")))

    set(s, "wxyz");
    clear(s);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("nil")))
    //]
  }

  { //[floating
    schematic s;

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("nil")))

    set(s, 17.5); 
    clear(s);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("nil")))
    //]
  }

  return boost::report_errors();
}
 
