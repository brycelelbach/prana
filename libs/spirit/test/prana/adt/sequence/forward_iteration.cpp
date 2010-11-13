/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_adt_sequence

#include <boost/spirit/home/prana/adt/sequence.hpp>

#include <fixture.hpp>

using namespace boost::spirit::prana;
using namespace boost::spirit::prana::test;

BOOST_FIXTURE_TEST_SUITE(unit_tests, fixture)

typedef boost::mpl::list<
  integer<0>,   floating<0>,   string<0, 4>,   string<0, 18>,
  integer<1>,   floating<1>,   string<1, 4>,   string<1, 18>,
  integer<2>,   floating<2>,   string<2, 4>,   string<2, 18>,
  integer<4>,   floating<4>,   string<4, 4>,   string<4, 18>,
  integer<8>,   floating<8>,   string<8, 4>,   string<8, 18>,
  integer<16>,  floating<16>,  string<16, 4>,  string<16, 18>,
  integer<32>,  floating<32>,  string<32, 4>,  string<32, 18>,
  integer<64>,  floating<64>,  string<64, 4>,  string<64, 18>
> elements;

BOOST_AUTO_TEST_CASE_TEMPLATE(forward_iteration, T, elements) {
  typedef sequence<typename T::type> sequence_type;

  typename T::container c;
  sequence_type s;

  BOOST_TEST_CHECKPOINT("default constructing sequence");
  s.default_construct();

  BOOST_TEST_CHECKPOINT(
    "filling sequence with " << T::elements::value << " elements"
  );
  for (std::size_t i = 0; i < T::elements::value; ++i) {
    typename T::type t = generator.get<T>();
    s.push_front(t);
    c.push_front(t);
    BOOST_TEST_MESSAGE("element " << i << ": " << c.back()); 
    BOOST_CHECK_EQUAL(*s.begin(), t); 
    BOOST_CHECK_EQUAL(*c.begin(), t); 
  }

  BOOST_TEST_CHECKPOINT("retrieving iterators");
  typename sequence_type::const_iterator sit = s.begin(), send = s.end();
  typename T::container::const_iterator cit = c.begin(), cend = c.end();

  BOOST_TEST_CHECKPOINT("verifying retrieved iterators");
  while ((sit != send) && (cit != cend)) {
    BOOST_CHECK_EQUAL(*cit, *sit); ++cit; ++sit;
  }
  
  BOOST_CHECK(sit == send);
  BOOST_CHECK(cit == cend);

  BOOST_TEST_CHECKPOINT("clearing sequence");
  s.clear();
}

BOOST_AUTO_TEST_SUITE_END()

