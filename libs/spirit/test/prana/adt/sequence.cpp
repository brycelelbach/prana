/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_adt_sequence

#include <boost/range/iterator_range.hpp>

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

typedef boost::mpl::list<
  integer<4, std::list>,      floating<4, std::list>,
  string<4, 4, std::list>,    string<4, 18, std::list>,
  integer<8, std::list>,      floating<8, std::list>,
  string<8, 4, std::list>,    string<8, 18, std::list>,
  integer<16, std::list>,     floating<16, std::list>,
  string<16, 4, std::list>,   string<16, 18, std::list>,
  integer<32, std::list>,     floating<32, std::list>,
  string<32, 4, std::list>,   string<32, 18, std::list>,
  integer<4, std::vector>,    floating<4, std::vector>,
  string<4, 4, std::vector>,  string<4, 18, std::vector>,
  integer<8, std::vector>,    floating<8, std::vector>,
  string<8, 4, std::vector>,  string<8, 18, std::vector>,
  integer<16, std::vector>,   floating<16, std::vector>,
  string<16, 4, std::vector>, string<16, 18, std::vector>,
  integer<32, std::vector>,   floating<32, std::vector>,
  string<32, 4, std::vector>, string<32, 18, std::vector>
> containers;

typedef boost::mpl::list<
  string<1, 1>,  string<1, 2>,  string<1, 3>,  string<1, 4>,
  string<1, 5>,  string<1, 6>,  string<1, 7>,  string<1, 8>,
  string<1, 9>,  string<1, 10>, string<1, 11>, string<1, 12>,
  string<1, 13>, string<1, 14>, string<1, 15>, string<1, 16>,
  string<1, 17>, string<1, 18>, string<1, 19>, string<1, 20>,
  string<1, 21>, string<1, 22>, string<1, 23>, string<1, 24>,
  string<1, 25>, string<1, 26>, string<1, 27>, string<1, 28>,
  string<1, 29>, string<1, 30>, string<1, 31>, string<1, 32> 
> strings;

typedef boost::mpl::list<bool, short, int, long> integers;

BOOST_AUTO_TEST_CASE_TEMPLATE(size, T, integers) {
  BOOST_CHECK_EQUAL(sizeof(void*[3]), sizeof(sequence<T>));
}

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

BOOST_AUTO_TEST_CASE_TEMPLATE(reverse_iteration, T, elements) {
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

  BOOST_TEST_CHECKPOINT("retrieving iterators");
  typename sequence_type::const_iterator sit = s.begin(), send = s.end();
  typename T::container::const_iterator cit = c.begin(), cend = c.end();

  BOOST_TEST_CHECKPOINT("verifying retrieved iterators");
  while ((sit != send) && (cit != cend)) {
    BOOST_CHECK_EQUAL(*--cend, *--send); 
  }

  BOOST_CHECK(sit == send);
  BOOST_CHECK(cit == cend);

  BOOST_TEST_CHECKPOINT("clearing sequence");
  s.clear();
}

BOOST_AUTO_TEST_CASE_TEMPLATE(front_removal, T, elements) {
  typedef sequence<typename T::type> sequence_type;

  typename T::container c;
  sequence_type s;

  BOOST_TEST_CHECKPOINT("default constructing sequence");
  s.default_construct();

  BOOST_TEST_CHECKPOINT(
    "filling sequence with " << (T::elements::value + 2) << " elements"
  );
  for (std::size_t i = 0; i < (T::elements::value + 2); ++i) {
    typename T::type t = generator.get<T>();
    s.push_front(t);
    c.push_front(t);
    BOOST_TEST_MESSAGE("element " << i << ": " << c.back()); 
    BOOST_CHECK_EQUAL(*s.begin(), t); 
    BOOST_CHECK_EQUAL(*c.begin(), t); 
  }
  
  BOOST_TEST_CHECKPOINT("removing first element (pop_front)");
  s.pop_front();
  c.pop_front();

  BOOST_TEST_CHECKPOINT("removing first element (erase)");
  s.erase(s.begin());
  c.erase(c.begin());

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

BOOST_AUTO_TEST_CASE_TEMPLATE(back_removal, T, elements) {
  typedef sequence<typename T::type> sequence_type;

  typename T::container c;
  sequence_type s;

  BOOST_TEST_CHECKPOINT("default constructing sequence");
  s.default_construct();

  BOOST_TEST_CHECKPOINT(
    "filling sequence with " << (T::elements::value + 2) << " elements"
  );
  for (std::size_t i = 0; i < (T::elements::value + 2); ++i) {
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

#if 0
BOOST_AUTO_TEST_CASE(deep_copy) {
  sequence<int> l0;
  l0.default_construct();
  l0.push_back(12);
  l0.push_back(53);
  l0.push_back(21);
  sequence<int> l1;
  l1.default_construct();
  l1.deep_copy(l0);
  sequence<int>::iterator it = l1.begin(), end = l1.end();
  BOOST_CHECK(*(--end) == 21);
  BOOST_CHECK(*(--end) == 53);
  BOOST_CHECK(*(--end) == 12);
  BOOST_CHECK(it == end);
  l1.clear();
  l0.clear(); 
}
 
BOOST_AUTO_TEST_CASE(get_to_list) {
  sequence<std::string> l0;
  l0.default_construct();
  l0.push_back("I'm");
  l0.push_back("a");
  l0.push_back("list");
  std::list<std::string> l1 = l0.get<std::list<std::string> >();
  l0.clear();
  std::list<std::string>::iterator it = l1.begin(), end = l1.end();
  BOOST_CHECK(*(it) == "I'm");
  BOOST_CHECK(*(++it) == "a");
  BOOST_CHECK(*(++it) == "list");
  BOOST_CHECK(++it == end);
}

BOOST_AUTO_TEST_CASE(get_to_vector) {
  sequence<double> l0;
  l0.default_construct();
  l0.push_back(32.5);
  l0.push_back(64.3);
  l0.push_back(12.0);
  std::vector<double> l1 = l0.get<std::vector<double> >();
  l0.clear(>);
  std::vector<double>::iterator it = l1.begin(), end = l1.end();
  BOOST_CHECK(*(it) == 32.5);
  BOOST_CHECK(*(++it) == 64.3);
  BOOST_CHECK(*(++it) == 12.0);
  BOOST_CHECK(++it == end);
}

BOOST_AUTO_TEST_CASE(get_to_range) {
  sequence<int> l;
  l.default_construct();
  l.push_back(6325);
  l.push_back(-503);
  l.push_back(10422);
  typedef boost::iterator_range<sequence<int>::const_iterator> irange;
  irange r = l.get<irange>();
  sequence<int>::const_iterator it = r.begin(), end = r.end();
  BOOST_CHECK(*(it) == 6325);
  BOOST_CHECK(*(++it) == -503);
  BOOST_CHECK(*(++it) == 10422);
  BOOST_CHECK(++it == end);
  l.clear();
}
#endif

BOOST_AUTO_TEST_SUITE_END()
