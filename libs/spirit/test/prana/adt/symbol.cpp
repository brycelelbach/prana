/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_adt_symbol

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform.hpp>

#include <boost/spirit/home/prana/adt/symbol.hpp>

#include <fixture.hpp>

using namespace boost::spirit::prana;
using namespace boost::spirit::prana::test;

BOOST_FIXTURE_TEST_SUITE(unit_tests, fixture)

typedef boost::mpl::list<
  symbol<char>, symbol<wchar_t>
> chars;

typedef boost::mpl::transform<
  boost::mpl::list_c<int,
    0,  1,  2,  3,  4,  5,  6,  7, 
    8,  9,  10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29, 30, 31
  >,
  boost::mpl::placeholders::_1
>::type lengths;

BOOST_AUTO_TEST_CASE_TEMPLATE(size, T, chars) {
  BOOST_CHECK_EQUAL(sizeof(void*[3]), sizeof(T));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(deep_copy_c_string_stack, T, lengths) {
  std::string str;
  symbol<char> sym;

  BOOST_TEST_CHECKPOINT("default constructing symbol");
  sym.default_construct();

  BOOST_TEST_CHECKPOINT(
    "creating a " << T::value << " length string"
  );
  str = generator.get<string<0, T::value> >();

  BOOST_TEST_CHECKPOINT("deep copying c-string into symbol");
  sym.deep_copy(str.c_str());

  BOOST_CHECK_EQUAL(sym.str(), str);

  BOOST_TEST_CHECKPOINT("clearing symbol");
}

#if 0
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
  BOOST_CHECK_EQUAL(h, "The long string is quite long.");
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
#endif

BOOST_AUTO_TEST_SUITE_END()
