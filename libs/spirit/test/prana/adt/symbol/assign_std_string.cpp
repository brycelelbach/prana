/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_adt_symbol

#include <fixture.hpp>

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform.hpp>

#include <boost/spirit/home/prana/adt/symbol.hpp>

using namespace boost::spirit::prana;
using namespace boost::spirit::prana::test;

BOOST_FIXTURE_TEST_SUITE(unit_tests, fixture)

typedef transform<
  list_c<int,
    0,  1,  2,  3,  4,  5,  6,  7, 
    8,  9,  10, 11, 12, 13, 14, 15
  >,
  placeholders::_1
>::type lengths;

BOOST_AUTO_TEST_CASE_TEMPLATE(assign_std_string, T, lengths) {
  std::string str;
  symbol<char> sym;

  BOOST_TEST_MESSAGE("default constructing symbol");
  sym.default_construct();

  BOOST_TEST_MESSAGE("generating a " << T::value << " length string");
  str = generator.get<string<0, T::value> >();

  BOOST_TEST_MESSAGE("deep copying std-string into symbol");
  sym.assign(str);

  BOOST_CHECK_EQUAL(sym.str(), str);

  BOOST_TEST_MESSAGE("clearing symbol");
  sym.clear();
}

BOOST_AUTO_TEST_SUITE_END()

