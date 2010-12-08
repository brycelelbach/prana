/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#include "harness.hpp"

#include <boost/spirit/home/prana/sexpr/core.hpp>
#include <boost/spirit/home/prana/sexpr/routine/clear.hpp>
#include <boost/spirit/home/prana/sexpr/routine/set.hpp>

int main (void) {
  using namespace boost::spirit::prana;

  { //[boolean
    sexpr s;

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("#nil")))

    set(s, true);
    clear(s);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("#nil")))
    //]
  }

  { //[ascii
    sexpr s;

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("#nil")))

    set(s, "wxyz");
    clear(s);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("#nil")))
    //]
  }

  { //[floating
    sexpr s;

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("#nil")))

    set(s, 17.5); 
    clear(s);

    BOOST_SPIRIT_PRANA_SEXPR_TESTS(
      ((s) ("#nil")))
    //]
  }

  return boost::report_errors();
}
 
