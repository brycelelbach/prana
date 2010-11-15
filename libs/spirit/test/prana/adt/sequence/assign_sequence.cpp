/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_adt_sequence

#include <fixture.hpp>

#include <boost/spirit/home/prana/adt/sequence.hpp>

using namespace boost::spirit::prana;
using namespace boost::spirit::prana::test;

BOOST_FIXTURE_TEST_SUITE(unit_tests, fixture)

typedef list<
  integer<2>,   floating<2>,   string<2, 4>,   string<2, 24>,
  integer<4>,   floating<4>,   string<4, 4>,   string<4, 24>,
  integer<6>,   floating<6>,   string<6, 4>,   string<6, 24>,
  integer<8>,   floating<8>,   string<8, 4>,   string<8, 24>
> elements;

BOOST_AUTO_TEST_CASE_TEMPLATE(assign_sequence, T, elements) {
  typedef sequence<typename T::type> sequence_type;

  typename T::container c;
  sequence_type s0, s1;

  BOOST_TEST_MESSAGE("default constructing sequences");
  s0.default_construct();
  s1.default_construct();

  BOOST_TEST_MESSAGE(
    "filling first sequence with " << T::elements::value << " elements"
  );
  for (std::size_t i = 0; i < T::elements::value; ++i) {
    typename T::type t = generator.get<T>();

    s0.push_back(t);
    c.push_back(t);
    BOOST_TEST_MESSAGE("element " << i << " is " << c.back()); 

    BOOST_CHECK_EQUAL(*--s0.end(), t); 
    BOOST_CHECK_EQUAL(*--c.end(), t); 
  }
  
  BOOST_TEST_MESSAGE("copying first sequence to second sequence");
  s1.assign(s0); 
 
  BOOST_TEST_MESSAGE("clearing first sequence");
  s0.clear();
 
  BOOST_TEST_MESSAGE("retrieving iterators");
  typename sequence_type::const_iterator sit = s1.begin(), send = s1.end();
  typename T::container::const_iterator cit = c.begin(), cend = c.end();

  BOOST_TEST_MESSAGE("verifying retrieved iterators");
  while ((sit != send) && (cit != cend)) {
    BOOST_CHECK_EQUAL(*cit, *sit); ++cit; ++sit;
  }
  
  BOOST_CHECK(sit == send);
  BOOST_CHECK(cit == cend);

  BOOST_TEST_MESSAGE("clearing second sequence");
  s1.clear();
}

BOOST_AUTO_TEST_SUITE_END()

