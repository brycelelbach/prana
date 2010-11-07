/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_adt_symbol

#include <boost/spirit/home/prana/adt/symbol.hpp>

#include <fixture.hpp>

using namespace boost::spirit::prana;

BOOST_FIXTURE_TEST_SUITE(unit_tests, test::fixture)

typedef boost::mpl::list<symbol<char>, symbol<wchar_t> > symbols;

BOOST_AUTO_TEST_CASE_TEMPLATE(size, T, symbols) {
  BOOST_CHECK_EQUAL(sizeof(void*[2]), sizeof(T));
}

BOOST_AUTO_TEST_CASE(deep_copy_char_string_stack) {
  symbol<char> s;
  s.default_construct();
  s.deep_copy("hello");
  BOOST_CHECK(s.str() == "hello");
  std::string h = s.get<std::string>();
  s.clear();
  BOOST_CHECK(h == "hello");
}

BOOST_AUTO_TEST_CASE(deep_copy_wchar_string_stack) {
  BOOST_TEST_MESSAGE(sizeof(symbol<wchar_t>));
  symbol<wchar_t> s;
  s.default_construct();
  s.deep_copy(L"hello");
  BOOST_CHECK(s.str() == L"hello");
  std::wstring h = s.get<std::wstring>();
  s.clear();
  BOOST_CHECK(h == L"hello");
}

BOOST_AUTO_TEST_CASE(deep_copy_char_string_heap) {
  symbol<char> s;
  s.default_construct();
  s.deep_copy("The long string is quite long.");
  BOOST_CHECK(s.str() == "The long string is quite long.");
  std::string h = s.get<std::string>();
  s.clear();
  BOOST_CHECK(h == "The long string is quite long.");
}

BOOST_AUTO_TEST_CASE(deep_copy_wchar_string_heap) {
  symbol<wchar_t> s;
  s.default_construct();
  s.deep_copy(L"The long string is quite long.");
  BOOST_CHECK(s.str() == L"The long string is quite long.");
  std::wstring h = s.get<std::wstring>();
  s.clear();
  BOOST_CHECK(h == L"The long string is quite long.");
}

BOOST_AUTO_TEST_CASE(deep_copy_empty_char_string) {
  symbol<char> s;
  s.default_construct();
  s.deep_copy("");
  BOOST_TEST_MESSAGE(s.str());
  BOOST_CHECK(s.str() == "");
  std::string h = s.get<std::string>();
  s.clear();
  BOOST_CHECK(h == "");
}

BOOST_AUTO_TEST_CASE(deep_copy_empty_wchar_string) {
  symbol<wchar_t> s;
  s.default_construct();
  s.deep_copy(L"");
  BOOST_CHECK(s.str() == L"");
  std::wstring h = s.get<std::wstring>();
  s.clear();
  BOOST_CHECK(h == L"");
}

BOOST_AUTO_TEST_SUITE_END()
