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
  integer<2>,   floating<2>,   string<2, 4>,   string<2, 18>,
  integer<3>,   floating<3>,   string<3, 4>,   string<3, 18>,
  integer<4>,   floating<4>,   string<4, 4>,   string<4, 18>,
  integer<5>,   floating<5>,   string<5, 4>,   string<5, 18>,
  integer<6>,   floating<6>,   string<6, 4>,   string<6, 18>,
  integer<12>,  floating<12>,  string<12, 4>,  string<12, 18>,
  integer<24>,  floating<24>,  string<24, 4>,  string<24, 18>,
  integer<48>,  floating<48>,  string<48, 4>,  string<48, 18>
> elements;

BOOST_AUTO_TEST_CASE_TEMPLATE(back_removal, T, elements) {
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
    s.push_back(t);
    c.push_back(t);
    BOOST_TEST_MESSAGE("element " << i << ": " << c.back()); 
    BOOST_CHECK_EQUAL(*--s.end(), t); 
    BOOST_CHECK_EQUAL(*--c.end(), t); 
  }
  
  BOOST_TEST_CHECKPOINT("removing last element (pop_back)");
  s.pop_back();
  c.pop_back();

  BOOST_TEST_CHECKPOINT("removing last element (erase)");
  s.erase(--s.end()); 
  c.erase(--c.end()); 
  
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

