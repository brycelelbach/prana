/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_adt_sequence

#include <boost/range/iterator_range.hpp>

#include <boost/spirit/home/prana/adt/sequence.hpp>

#include <fixture.hpp>

using namespace boost::spirit::prana;
using namespace boost::spirit::prana::adt;

BOOST_AUTO_TEST_CASE(insertion) {
  sequence<int> l;
  l.default_construct();
  l.push_back(15);
  l.push_back(2134);
  l.push_back(-423);
  l.free();
}

BOOST_AUTO_TEST_CASE(forward_iteration) {
  sequence<double> l;
  l.default_construct();
  l.insert(7005.995, l.begin());
  l.insert(153.51, l.begin());
  l.insert(0.445, l.begin());
  sequence<double>::iterator it = l.begin(), end = l.end();
  BOOST_CHECK(*it == 0.445);
  BOOST_CHECK(*(++it) == 153.51);
  BOOST_CHECK(*(++it) == 7005.995);
  BOOST_CHECK(++it == end);
  l.free();
}

BOOST_AUTO_TEST_CASE(reverse_iteration) {
  sequence<std::string> l;
  l.default_construct();
  l.insert("foo", l.end());
  l.insert("bar", l.end());
  l.insert("buzz", l.end());
  sequence<std::string>::iterator it = l.begin(), end = l.end();
  BOOST_CHECK(*(--end) == "buzz");
  BOOST_CHECK(*(--end) == "bar");
  BOOST_CHECK(*(--end) == "foo");
  BOOST_CHECK(it == end);
  l.free();
}

BOOST_AUTO_TEST_CASE(front_removal) {
  sequence<char> l;
  l.default_construct();
  l.push_front('c');
  l.push_front('b');
  l.push_front('a');
  l.pop_front();
  sequence<char>::iterator it = l.begin(), end = l.end();
  it = l.erase(it);
  BOOST_CHECK(*(it) == 'c');
  BOOST_CHECK(++it == end);
  l.free();
}

BOOST_AUTO_TEST_CASE(back_removal) {
  sequence<short> l;
  l.default_construct();
  l.push_back(12);
  l.push_back(17);
  l.push_back(-5);
  l.pop_back();
  sequence<short>::iterator it = l.begin(), end = l.end();
  end = l.erase(--end);
  BOOST_CHECK(*(--end) == 12);
  BOOST_CHECK(it == end);
  l.free();
}

BOOST_AUTO_TEST_CASE(iterator_removal) {
  sequence<std::string> l;
  l.default_construct();
  l.push_back("hello");
  l.push_back("world");
  l.push_back("!");
  sequence<std::string>::iterator it = l.begin(), end = l.end();
  BOOST_CHECK(*(it) == "hello");
  it = l.erase(++it);
  BOOST_CHECK(*(it) == "!");
  BOOST_CHECK(++it == end);
  l.free();
}

BOOST_AUTO_TEST_CASE(deep_copy) {
  sequence<int> l0;
  l0.default_construct();
  l0.push_back(12);
  l0.push_back(53);
  l0.push_back(21);
  sequence<int> l1;
  l1.default_construct();
  l1.deep_copy(l0);
  sequence<int>::iterator it = l1.begin(), end = l1.end();
  BOOST_CHECK(*(--end) == 21);
  BOOST_CHECK(*(--end) == 53);
  BOOST_CHECK(*(--end) == 12);
  BOOST_CHECK(it == end);
  l1.free();
  l0.free(); 
}
 
BOOST_AUTO_TEST_CASE(get_to_list) {
  sequence<std::string> l0;
  l0.default_construct();
  l0.push_back("I'm");
  l0.push_back("a");
  l0.push_back("list");
  std::list<std::string> l1 = l0.get<std::list<std::string> >();
  l0.free();
  std::list<std::string>::iterator it = l1.begin(), end = l1.end();
  BOOST_CHECK(*(it) == "I'm");
  BOOST_CHECK(*(++it) == "a");
  BOOST_CHECK(*(++it) == "list");
  BOOST_CHECK(++it == end);
}

BOOST_AUTO_TEST_CASE(get_to_vector) {
  sequence<double> l0;
  l0.default_construct();
  l0.push_back(32.5);
  l0.push_back(64.3);
  l0.push_back(12.0);
  std::vector<double> l1 = l0.get<std::vector<double> >();
  l0.free();
  std::vector<double>::iterator it = l1.begin(), end = l1.end();
  BOOST_CHECK(*(it) == 32.5);
  BOOST_CHECK(*(++it) == 64.3);
  BOOST_CHECK(*(++it) == 12.0);
  BOOST_CHECK(++it == end);
}

BOOST_AUTO_TEST_CASE(get_to_range) {
  sequence<int> l;
  l.default_construct();
  l.push_back(6325);
  l.push_back(-503);
  l.push_back(10422);
  typedef boost::iterator_range<sequence<int>::const_iterator> irange;
  irange r = l.get<irange>();
  sequence<int>::const_iterator it = r.begin(), end = r.end();
  BOOST_CHECK(*(it) == 6325);
  BOOST_CHECK(*(++it) == -503);
  BOOST_CHECK(*(++it) == 10422);
  BOOST_CHECK(++it == end);
  l.free();
}

