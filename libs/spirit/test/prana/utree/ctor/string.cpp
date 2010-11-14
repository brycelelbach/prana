/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_utree

#include <fixture.hpp>

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform.hpp>

#include <boost/spirit/home/prana/utree.hpp>

using namespace boost::spirit::prana;
using namespace boost::spirit::prana::test;

BOOST_FIXTURE_TEST_SUITE(unit_tests, fixture)

typedef list<
  string<8, 2>,  string<8, 4>,  string<8, 6>,  string<8, 8>,
  string<8, 16>, string<8, 24>, string<8, 32>, string<8, 64>
> strings;

BOOST_AUTO_TEST_CASE_TEMPLATE(std_string_ctor, T, strings) {
  for (std::size_t i = 0; i < T::elements::value; ++i) {
    typename T::type str(generator.get<T>());
    
    BOOST_TEST_MESSAGE("constructing utree with string " << str);
    utree t(str);

    BOOST_CHECK_EQUAL(t.template get<typename T::type>(), str);
  }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(c_string_ctor, T, strings) {
  for (std::size_t i = 0; i < T::elements::value; ++i) {
    typename T::type str(generator.get<T>());
    
    BOOST_TEST_MESSAGE("constructing utree with string " << str);
    utree t(str.c_str());

    BOOST_CHECK_EQUAL(t.template get<typename T::type>(), str);
  }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(string_iterator_ctor, T, strings) {
  for (std::size_t i = 0; i < T::elements::value; ++i) {
    typename T::type str(generator.get<T>());
    
    BOOST_TEST_MESSAGE("constructing utree with string " << str);
    utree t(str.begin(), str.end());

    BOOST_CHECK_EQUAL(t.template get<typename T::type>(), str);
  }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bits_plus_len_ctor, T, strings) {
  for (std::size_t i = 0; i < T::elements::value; ++i) {
    typename T::type str(generator.get<T>()),
                     substr(str.begin(), str.begin() + (T::length::value / 2));
    
    BOOST_TEST_MESSAGE("constructing utree with string " << str);
    utree t(str.c_str(), T::length::value / 2);

    BOOST_CHECK_EQUAL(t.template get<typename T::type>(), substr);
  }
}

BOOST_AUTO_TEST_SUITE_END()
