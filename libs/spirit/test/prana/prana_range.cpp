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
using namespace boost::spirit::prana::adt;

BOOST_FIXTURE_TEST_SUITE(prana_range, test::fixture) 

BOOST_AUTO_TEST_CASE_TEMPLATE(deep_copy_list, T, test::rounds) {
  typedef std::list<typename T::type> list_type;
  typedef range<typename list_type::const_iterator> range_type;

  list_type l;
  range_type r;

  BOOST_TEST_CHECKPOINT("default constructing range");
  r.default_construct();

  BOOST_TEST_CHECKPOINT("generating 5 random list elements"); 
  typename T::type a = random<T>(),
                   b = random<T>(),
                   c = random<T>(),
                   d = random<T>(),
                   e = random<T>();

  BOOST_TEST_MESSAGE("a: " << a);
  BOOST_TEST_MESSAGE("b: " << b);
  BOOST_TEST_MESSAGE("c: " << c);
  BOOST_TEST_MESSAGE("d: " << d);
  BOOST_TEST_MESSAGE("e: " << e);

  BOOST_TEST_CHECKPOINT("inserting elements into list"); 
  l.push_back(a);
  l.push_back(b);
  l.push_back(c);
  l.push_back(d);
  l.push_back(e);

  BOOST_TEST_CHECKPOINT("deep copying list into range"); 
  r.deep_copy(l);

  BOOST_TEST_CHECKPOINT("retrieving range iterators"); 
  typename range_type::iterator it = r.begin(), end = r.end();

  BOOST_TEST_CHECKPOINT("verifying list data");
  BOOST_CHECK(*(--end) == e);
  BOOST_CHECK(*(--end) == d);
  BOOST_CHECK(*(--end) == c);
  BOOST_CHECK(*(--end) == b);
  BOOST_CHECK(*(--end) == a);
  BOOST_CHECK(it == end);

  BOOST_TEST_CHECKPOINT("freeing range");
  r.free();
}
 
BOOST_AUTO_TEST_CASE(deep_copy_c_str) {
  char const* hello = "hello";
  range<char const*> r;
  r.default_construct();
  r.deep_copy(hello, hello + std::strlen(hello));
  std::string s(r.begin(), r.end());
  r.free();
  BOOST_CHECK(s == "hello");
}

BOOST_AUTO_TEST_CASE(deep_copy_range) {
  std::list<char> l;
  range<std::list<char>::const_iterator> r0, r1;
  r0.default_construct();
  r1.default_construct();
  l.push_back('z');
  l.push_back('f');
  l.push_back('b');
  r0.deep_copy(l);
  r1.deep_copy(r0);
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
  r.deep_copy(v0);
  std::vector<std::string> v1 = r.get<std::vector<std::string> >();
  std::vector<std::string>::iterator it = v1.begin(), end = v1.end();
  r.free();
  BOOST_CHECK(*(--end) == "train");
  BOOST_CHECK(*(--end) == "bus");
  BOOST_CHECK(*(--end) == "car");
  BOOST_CHECK(it == end);
}

BOOST_AUTO_TEST_SUITE_END()
