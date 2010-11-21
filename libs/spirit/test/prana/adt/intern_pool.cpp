/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_intern_pool

#include <fixture.hpp>

#include <boost/spirit/home/prana/adt/intern_pool.hpp> 

using namespace boost::spirit::prana;
using namespace boost::spirit::prana::test;

BOOST_FIXTURE_TEST_SUITE(unit_tests, fixture)

typedef list<
  string<4, 2>,  string<4, 4>,  string<4, 16>,  string<4, 32>,
  string<8, 2>,  string<8, 4>,  string<8, 16>,  string<8, 32>
> strings;

BOOST_AUTO_TEST_CASE_TEMPLATE(intern_std_string, T, strings) {
  typedef intern_pool<std::string::iterator> pool;
  
  pool p;

  for (std::size_t i = 0; i < T::elements::value; ++i) {
    typename T::type str(generator.get<T>());
    
    BOOST_TEST_MESSAGE("inserting string " << str << " as element " << i);

    p.insert(str.begin(), str.end());

    typename T::type::iterator it = str.begin();

    BOOST_CHECK_EQUAL(bool(p.find(it, str.end())), true);
    BOOST_CHECK(it == str.end());
    
    p.erase(str.begin(), str.end());

    p.insert(str.begin(), str.begin() + 2);

    it = str.begin();

    typename pool::pointer r = 0;

    BOOST_CHECK_EQUAL(bool(r = p.find(it, str.end())), true);
    BOOST_CHECK_EQUAL(*r->begin(), *str.begin());
    BOOST_CHECK_EQUAL(*r->end(), *it);
    BOOST_CHECK_EQUAL(*it, *(str.begin() + 2));
  }
}

BOOST_AUTO_TEST_SUITE_END()

