/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/adt/dynamic_array.hpp> 

int main (void) { try {
  using boost::spirit::prana::dynamic_array;

  typedef dynamic_array<unsigned> array_type;

  { 
    array_type t;

    // insertion
    t.push(1);
    t.push(2);
    t.push(3);
    BOOST_TEST_EQ(t.size(), 3);

    // lookup
    BOOST_TEST_EQ(t[0], 1);
    BOOST_TEST_EQ(t[1], 2);
    BOOST_TEST_EQ(t[2], 3);

    // removal
    t.pop();
    t.pop();
    t.pop();
    BOOST_TEST_EQ(t.size(), 0);
  }
  
  { 
    array_type t;

    // insertion
    for (unsigned i = 0; i < 8; ++i) { t.push(i + 1); }
    BOOST_TEST_EQ(t.size(), 8);
    BOOST_TEST_EQ(t.capacity(), 8);
    t.push(9);
    BOOST_TEST_EQ(t.size(), 9);
    BOOST_TEST_EQ(t.capacity(), 16);

    // lookup
    for (unsigned i = 0; i < 9; ++i) { BOOST_TEST_EQ(t[i], (i + 1)); }

    // removal
    t.clear();
    BOOST_TEST_EQ(t.size(), 0);
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}

