/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_symbol_table

#include <fixture.hpp>

#include <boost/spirit/home/prana/adt/symbol_table.hpp> 

using namespace boost;
using namespace boost::spirit::prana;
using namespace boost::spirit::prana::test;

BOOST_FIXTURE_TEST_SUITE(unit_tests, fixture)

typedef list<
  integer<2>, floating<2>, string<2, 4>, string<2, 24>,
  integer<4>, floating<4>, string<4, 4>, string<4, 24>,
  integer<6>, floating<6>, string<6, 4>, string<6, 24>,
  integer<8>, floating<8>, string<8, 4>, string<8, 24>
> elements;

BOOST_AUTO_TEST_CASE_TEMPLATE(symbol_std_string, T, elements) {
  typedef symbol_table<std::string::iterator, typename T::type> pool;
  
  pool p;

  for (std::size_t i = 0; i < T::elements::value; ++i) {
    std::string str(generator.get<string<1, T::elements::value> >());
    typename T::type data(generator.get<T>());
    
    BOOST_TEST_MESSAGE("inserting " << data << " as " << str);

    p.insert(str.begin(), str.end(), data);

    std::string::iterator it = str.begin();

    BOOST_CHECK(bool(p.find(it, str.end())) == true);
    BOOST_CHECK(it == str.end());
    
    p.erase(str.begin(), str.end());

    p.insert(str.begin(), str.begin() + 2, data);

    it = str.begin();

    typename pool::pointer r = 0;

    BOOST_CHECK(bool(r = p.find(it, str.end())) == true);
    BOOST_CHECK(*fusion::at_c<0>(*r).begin() == *str.begin());
    BOOST_CHECK(*fusion::at_c<0>(*r).end() == *it);
    BOOST_CHECK(fusion::at_c<1>(*r) == data);
    BOOST_CHECK(*it == *(str.begin() + 2));
  }
}

BOOST_AUTO_TEST_SUITE_END()

