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

typedef transform<
  list_c<int,
    0,  1,  2,  3,  4,  5,  6,  7, 
    8,  9,  10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29, 30, 31
  >,
  placeholders::_1
>::type lengths;

typedef transform<
  list_c<int,
    2,  4,  6,  8,  10, 12, 14, 16,
    18, 20, 22, 24, 26, 28, 30, 32,
    34, 36, 38, 40, 42, 44, 46, 48,
    50, 52, 54, 56, 58, 60, 62, 64
  >,
  placeholders::_1
>::type evens;

BOOST_AUTO_TEST_CASE_TEMPLATE(deep_copy_c_string, T, lengths) {
  std::string str;
  symbol<char> sym;

  BOOST_TEST_MESSAGE("default constructing symbol");
  sym.default_construct();

  BOOST_TEST_MESSAGE("generating a " << T::value << " length string");
  str = generator.get<string<0, T::value> >();

  BOOST_TEST_MESSAGE("deep copying c-string into symbol");
  sym.deep_copy(str.c_str());

  BOOST_CHECK_EQUAL(sym.str(), str);

  BOOST_TEST_MESSAGE("clearing symbol");
  sym.clear();
}

BOOST_AUTO_TEST_CASE_TEMPLATE(deep_copy_std_string, T, lengths) {
  std::string str;
  symbol<char> sym;

  BOOST_TEST_MESSAGE("default constructing symbol");
  sym.default_construct();

  BOOST_TEST_MESSAGE("generating a " << T::value << " length string");
  str = generator.get<string<0, T::value> >();

  BOOST_TEST_MESSAGE("deep copying std-string into symbol");
  sym.deep_copy(str);

  BOOST_CHECK_EQUAL(sym.str(), str);

  BOOST_TEST_MESSAGE("clearing symbol");
  sym.clear();
}

BOOST_AUTO_TEST_CASE_TEMPLATE(deep_copy_half_c_string, T, lengths) {
  std::string str;
  symbol<char> sym;

  BOOST_TEST_MESSAGE("default constructing symbol");
  sym.default_construct();

  BOOST_TEST_MESSAGE("generating a " << T::value << " length string");
  str = generator.get<string<0, T::value> >();

  BOOST_TEST_MESSAGE("deep copying c-string into symbol");
  sym.deep_copy(str.c_str(), str.c_str() + (T::value / 2));

  BOOST_TEST_MESSAGE("forming " << T::value << " length substring");
  std::string substr(str.begin(), str.begin() + (T::value / 2));

  BOOST_CHECK_EQUAL(sym.str(), substr);

  BOOST_TEST_MESSAGE("clearing symbol");
  sym.clear();
}

BOOST_AUTO_TEST_CASE_TEMPLATE(deep_copy_half_std_string, T, lengths) {
  std::string str;
  symbol<char> sym;

  BOOST_TEST_MESSAGE("default constructing symbol");
  sym.default_construct();

  BOOST_TEST_MESSAGE("generating a " << T::value << " length string");
  str = generator.get<string<0, T::value> >();

  BOOST_TEST_MESSAGE("deep copying c-string into symbol");
  sym.deep_copy(str.begin(), str.begin() + (T::value / 2));

  BOOST_TEST_MESSAGE("forming " << T::value << " length substring");
  std::string substr(str.begin(), str.begin() + (T::value / 2));

  BOOST_CHECK_EQUAL(sym.str(), substr);

  BOOST_TEST_MESSAGE("clearing symbol");
  sym.clear();
}

BOOST_AUTO_TEST_SUITE_END()

