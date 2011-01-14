/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>
#include <boost/spirit/home/prana/schematic/routine/clear.hpp>
#include <boost/spirit/home/prana/schematic/routine/set.hpp>

int main (void) { try {
  using boost::spirit::prana::schematic;
  using boost::spirit::prana::set;
  using boost::spirit::prana::clear;

  { //[boolean
    schematic s;

    BOOST_SPIRIT_PRANA_SCHEMATIC_TESTS(
      ((s) ("nil")))

    set(s, true);
    clear(s);

    BOOST_SPIRIT_PRANA_SCHEMATIC_TESTS(
      ((s) ("nil")))
    //]
  }

  { //[ascii
    schematic s;

    BOOST_SPIRIT_PRANA_SCHEMATIC_TESTS(
      ((s) ("nil")))

    set(s, "wxyz");
    clear(s);

    BOOST_SPIRIT_PRANA_SCHEMATIC_TESTS(
      ((s) ("nil")))
    //]
  }

  { //[floating
    schematic s;

    BOOST_SPIRIT_PRANA_SCHEMATIC_TESTS(
      ((s) ("nil")))

    set(s, 17.5); 
    clear(s);

    BOOST_SPIRIT_PRANA_SCHEMATIC_TESTS(
      ((s) ("nil")))
    //]
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
