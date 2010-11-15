/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_utree

#include <fixture.hpp>

#include <boost/spirit/home/prana/utree.hpp>

using namespace boost::spirit::prana;
using namespace boost::spirit::prana::test;

BOOST_FIXTURE_TEST_SUITE(unit_tests, fixture)

typedef list<
  type_<6, bool>, type_<6, short>, type_<6, intmax_t>, type_<6, double>,
  string<6, 2>,   string<6, 8>,    string<6, 16>,      string<6, 32>
> numbers;

BOOST_AUTO_TEST_CASE_TEMPLATE(numeric_ctor, T, numbers) {
  for (std::size_t i = 0; i < T::elements::value; ++i) {
    typename T::type n = generator.get<T>();
    
    BOOST_TEST_MESSAGE("constructing first utree with value " << n);
    utree t0(n);

    BOOST_TEST_MESSAGE("copy constructing second utree from first utree");
    utree t1(t0);

    BOOST_TEST_MESSAGE("clearing first utree");
    t0.clear();

    BOOST_CHECK(t1 == n);
  }
}

BOOST_AUTO_TEST_SUITE_END()
