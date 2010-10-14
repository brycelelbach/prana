/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_dllist

#include <boost/spirit/home/prana/adt/dllist.hpp>
#include <boost/test/unit_test.hpp>

using namespace boost::spirit::prana;

BOOST_AUTO_TEST_CASE(default_and_free) {
  dllist<int> l;
  l.default_construct();
  l.free();
}

BOOST_AUTO_TEST_CASE(insertion) {
  dllist<int> l;
  l.default_construct();
  l.push_back(15);
  l.push_back(2134);
  l.push_back(-423);
  l.free();
}

BOOST_AUTO_TEST_CASE(forward_iteration) {
  dllist<double> l;
  l.default_construct();
  l.push_front(7005.995);
  l.push_front(153.51);
  l.push_front(0.445);
  dllist<double>::iterator it = l.begin(), end = l.end();
  BOOST_CHECK(*it == 0.445);
  BOOST_CHECK(*(++it) == 153.51);
  BOOST_CHECK(*(++it) == 7005.995);
  BOOST_CHECK(++it == end);
  l.free();
}

BOOST_AUTO_TEST_CASE(reverse_iteration) {
  dllist<std::string> l;
  l.default_construct();
  l.push_back("foo");
  l.push_back("bar");
  l.push_back("buzz");
  dllist<std::string>::iterator it = l.begin(), end = l.end();
  BOOST_CHECK(*(--end) == "buzz");
  BOOST_CHECK(*(--end) == "bar");
  BOOST_CHECK(*(--end) == "foo");
  BOOST_CHECK(it == end);
  l.free();
}

BOOST_AUTO_TEST_CASE(front_removal) {
  dllist<char> l;
  l.default_construct();
  l.push_front('c');
  l.push_front('b');
  l.push_front('a');
  l.pop_front();
  dllist<char>::iterator it = l.begin(), end = l.end();
  it = l.erase(it);
  BOOST_CHECK(*(it) == 'c');
  BOOST_CHECK(++it == end);
  l.free();
}

BOOST_AUTO_TEST_CASE(back_removal) {
  dllist<short> l;
  l.default_construct();
  l.push_back(12);
  l.push_back(17);
  l.push_back(-5);
  l.pop_back();
  dllist<short>::iterator it = l.begin(), end = l.end();
  end = l.erase(--end);
  BOOST_CHECK(*(--end) == 12);
  BOOST_CHECK(it == end);
  l.free();
}

BOOST_AUTO_TEST_CASE(iterator_removal) {
  dllist<std::string> l;
  l.default_construct();
  l.push_back("hello");
  l.push_back("world");
  l.push_back("!");
  dllist<std::string>::iterator it = l.begin(), end = l.end();
  BOOST_CHECK(*(it) == "hello");
  it = l.erase(++it);
  BOOST_CHECK(*(it) == "!");
  BOOST_CHECK(++it == end);
  l.free();
}

