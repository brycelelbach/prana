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
    t.push_back(1);
    t.push_back(2);
    t.push_back(3);
    BOOST_TEST_EQ(t.size(), 3);

    // lookup
    BOOST_TEST_EQ(t[0], 1);
    BOOST_TEST_EQ(t[1], 2);
    BOOST_TEST_EQ(t[2], 3);

    // removal
    t.pop_back();
    t.pop_back();
    t.pop_back();
    BOOST_TEST_EQ(t.size(), 0);
  }
  
  { 
    array_type t;

    // insertion
    for (unsigned i = 0; i < 8; ++i) { t.push_back(i + 1); }
    BOOST_TEST_EQ(t.size(), 8);
    BOOST_TEST_EQ(t.capacity(), 8);
    t.push_back(9);
    BOOST_TEST_EQ(t.size(), 9);
    BOOST_TEST_EQ(t.capacity(), 16);

    // lookup
    for (unsigned i = 0; i < 9; ++i) { BOOST_TEST_EQ(t[i], (i + 1)); }

    // removal
    t.clear();
    BOOST_TEST_EQ(t.size(), 0);
  }

  { 
    array_type t0, t1;

    // insertion
    t0.push_back(1);
    t0.push_back(2);
    t0.push_back(3);
    BOOST_TEST_EQ(t0.size(), 3);
    BOOST_TEST_EQ(t1.size(), 0);

    // copy
    t1 = t0;
    BOOST_TEST_EQ(t0.size(), 3);
    BOOST_TEST_EQ(t1.size(), 3);

    // clear first
    t0.clear();
    BOOST_TEST_EQ(t0.size(), 0);
    BOOST_TEST_EQ(t1.size(), 3);

    // lookup
    BOOST_TEST_EQ(t1[0], 1);
    BOOST_TEST_EQ(t1[1], 2);
    BOOST_TEST_EQ(t1[2], 3);

    // removal
    t1.pop_back();
    t1.pop_back();
    t1.pop_back();
    BOOST_TEST_EQ(t0.size(), 0);
    BOOST_TEST_EQ(t1.size(), 0);
  }
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}

