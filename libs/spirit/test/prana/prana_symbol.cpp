/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_symbol

#include <cstddef>

#include <vector>
#include <list>

#include <boost/spirit/home/prana/adt/symbol.hpp>

#include "prana_test_harness.hpp"

using namespace boost::spirit::prana::adt;

BOOST_AUTO_TEST_CASE(ctors_and_dtors) {
  symbol<char> s;
  symbol<wchar_t> w;
  s.default_construct();
  w.default_construct();
  s.free();
  w.free();
  s = symbol<char>::make();
  w = symbol<wchar_t>::make();
  s.free();
  s.free();
  w.free();
  w.free();
}

BOOST_AUTO_TEST_CASE(copy_char_string_stack) {
  symbol<char> s;
  s.default_construct();
  s.copy("hello");
  BOOST_CHECK(s.str() == "hello");
  std::string h = s.get<std::string>();
  s.free();
  BOOST_CHECK(h == "hello");
}

BOOST_AUTO_TEST_CASE(copy_wchar_string_stack) {
  symbol<wchar_t> s;
  s.default_construct();
  s.copy(L"hello");
  BOOST_CHECK(s.str() == L"hello");
  std::wstring h = s.get<std::wstring>();
  s.free();
  BOOST_CHECK(h == L"hello");
}

BOOST_AUTO_TEST_CASE(copy_char_string_heap) {
  symbol<char> s;
  s.default_construct();
  s.copy("The long string is quite long.");
  BOOST_CHECK(s.str() == "The long string is quite long.");
  std::string h = s.get<std::string>();
  s.free();
  BOOST_CHECK(h == "The long string is quite long.");
}

BOOST_AUTO_TEST_CASE(copy_wchar_string_heap) {
  symbol<wchar_t> s;
  s.default_construct();
  s.copy(L"The long string is quite long.");
  BOOST_CHECK(s.str() == L"The long string is quite long.");
  std::wstring h = s.get<std::wstring>();
  s.free();
  BOOST_CHECK(h == L"The long string is quite long.");
}

BOOST_AUTO_TEST_CASE(copy_empty_char_string) {
  symbol<char> s;
  s.default_construct();
  s.copy("");
  BOOST_TEST_MESSAGE(s.str());
  BOOST_CHECK(s.str() == "");
  std::string h = s.get<std::string>();
  s.free();
  BOOST_CHECK(h == "");
}

BOOST_AUTO_TEST_CASE(copy_empty_wchar_string) {
  symbol<wchar_t> s;
  s.default_construct();
  s.copy(L"");
  BOOST_CHECK(s.str() == L"");
  std::wstring h = s.get<std::wstring>();
  s.free();
  BOOST_CHECK(h == L"");
}

