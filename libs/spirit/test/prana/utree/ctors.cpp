/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_utree_core

#include <boost/spirit/home/prana/utree.hpp>

#include <fixture.hpp>

using namespace boost::spirit::prana;
using namespace boost::spirit::prana::test;

BOOST_FIXTURE_TEST_SUITE(unit_tests, fixture)

typedef list<
  type_<8, bool>,
  type_<8, uint16_t>,
  type_<8, int16_t>,
  type_<8, uint32_t>,
  type_<8, int32_t>,
  type_<8, intmax_t>,
  type_<8, float>,
  type_<8, double>
> numbers;

BOOST_AUTO_TEST_CASE_TEMPLATE(numeric_ctors, T, numbers) {
  for (std::size_t i = 0; i < T::elements::value; ++i) {
    typename T::type n = generator.get<T>();
    BOOST_TEST_MESSAGE("value " << i << ": " << n); 
    
    BOOST_TEST_MESSAGE("constructing utree with numeric value " << n);
    utree t(n);

    BOOST_CHECK(t == n);
  }
}

BOOST_AUTO_TEST_SUITE_END()
