/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_range

#include <cstring>

#include <vector>
#include <list>

#include <boost/spirit/home/prana/adt/range.hpp>

#include "prana_test_harness.hpp"

using namespace boost::spirit::prana;

BOOST_AUTO_TEST_CASE(ctors_and_dtors) {
  range<std::vector<int>::const_iterator> r;
  r.default_construct();
  r.free();
  r = range<std::vector<int>::const_iterator>::make();
  r.free();
  r.free();
}

BOOST_AUTO_TEST_CASE(copy_list) {
  std::list<short> l;
  range<std::list<short>::const_iterator> r;
  r.default_construct();
  l.push_back(12);
  l.push_back(53);
  l.push_back(21);
  r.copy(l);
  range<std::list<short>::const_iterator>::iterator
    it = r.begin(), end = r.end();
  BOOST_CHECK(*(--end) == 21);
  BOOST_CHECK(*(--end) == 53);
  BOOST_CHECK(*(--end) == 12);
  BOOST_CHECK(it == end);
  r.free();
}
 
BOOST_AUTO_TEST_CASE(copy_c_str) {
  char const* hello = "hello";
  range<char const*> r;
  r.default_construct();
  r.copy(hello, hello + std::strlen(hello));
  std::string s(r.begin(), r.end());
  r.free();
  BOOST_CHECK(s == "hello");
}

BOOST_AUTO_TEST_CASE(copy_range) {
  std::list<char> l;
  range<std::list<char>::const_iterator> r0, r1;
  r0.default_construct();
  r1.default_construct();
  l.push_back('z');
  l.push_back('f');
  l.push_back('b');
  r0.copy(l);
  r1.copy(r0);
  r0.free();
  range<std::list<char>::const_iterator>::iterator
    it = r1.begin(), end = r1.end();
  BOOST_CHECK(*(--end) == 'b');
  BOOST_CHECK(*(--end) == 'f');
  BOOST_CHECK(*(--end) == 'z');
  BOOST_CHECK(it == end);
  r1.free();
  r0.free();
}

BOOST_AUTO_TEST_CASE(get_to_vector) {
  std::vector<std::string> v0;
  range<std::vector<std::string>::const_iterator> r;
  r.default_construct();
  v0.push_back("car");
  v0.push_back("bus");
  v0.push_back("train");
  r.copy(v0);
  std::vector<std::string> v1 = r.get<std::vector<std::string> >();
  std::vector<std::string>::iterator it = v1.begin(), end = v1.end();
  r.free();
  BOOST_CHECK(*(--end) == "train");
  BOOST_CHECK(*(--end) == "bus");
  BOOST_CHECK(*(--end) == "car");
  BOOST_CHECK(it == end);
}
