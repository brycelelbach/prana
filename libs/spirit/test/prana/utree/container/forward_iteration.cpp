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
  integer<0>,   floating<0>,    string<0, 4>,    string<0, 24>,
  integer<4>,   floating<4>,    string<4, 4>,    string<4, 24>,
  integer<8>,   floating<8>,    string<8, 4>,    string<8, 24>,
  integer<16>,  floating<16>,   string<16, 4>,   string<16, 24>
> elements;

BOOST_AUTO_TEST_CASE_TEMPLATE(forward_iteration, T, elements) {
  utree s;
  typename T::container c;

  BOOST_TEST_MESSAGE(
    "filling utree with " << T::elements::value << " elements"
  );
  for (std::size_t i = 0; i < T::elements::value; ++i) {
    typename T::type v = generator.get<T>();
    utree t(v);

    s.push_front(t);
    c.push_front(t.template get<typename T::type>());
    BOOST_TEST_MESSAGE("element " << i << " is " << c.back()); 

    //BOOST_CHECK(*s.begin() == t); 
    //BOOST_CHECK_EQUAL(*c.begin(), t.template get<typename T::type>()); 
  }

  BOOST_TEST_MESSAGE("retrieving iterators");
  typename utree::const_iterator sit = s.begin(), send = s.end();
  typename T::container::const_iterator cit = c.begin(), cend = c.end();

  BOOST_TEST_MESSAGE("verifying retrieved iterators");
  while ((sit != send) && (cit != cend)) {
    BOOST_CHECK_EQUAL(*cit, (*sit).template get<typename T::type>());
    ++cit; ++sit;
  }
  
  BOOST_CHECK(sit == send);
  BOOST_CHECK(cit == cend);

  BOOST_TEST_MESSAGE("clearing utree");
  s.clear();
}

BOOST_AUTO_TEST_SUITE_END()

