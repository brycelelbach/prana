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
using namespace boost::spirit::prana::test;

BOOST_FIXTURE_TEST_SUITE(unit_tests, fixture)

typedef boost::mpl::list<
  symbol<char>, symbol<wchar_t>
> chars;

BOOST_AUTO_TEST_CASE_TEMPLATE(size, T, chars) {
  BOOST_CHECK_EQUAL(sizeof(void*[3]), sizeof(T));
}

BOOST_AUTO_TEST_SUITE_END()
