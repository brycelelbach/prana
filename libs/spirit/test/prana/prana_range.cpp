/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_range

#include <cstring>

#include <vector>
#include <list>

#include <boost/spirit/home/prana/adt/range.hpp>

#include "prana_test_harness.hpp"

using namespace boost::spirit::prana;
using namespace boost::spirit::prana::adt;

BOOST_FIXTURE_TEST_SUITE(prana_range, test::fixture) 

BOOST_AUTO_TEST_CASE_TEMPLATE(deep_copy_list, T, test::rounds) {
  typedef std::list<typename T::type> list_type;
  typedef range<typename list_type::const_iterator> range_type;

  list_type l;
  range_type r;

  BOOST_TEST_CHECKPOINT("default constructing range");
  r.default_construct();

  BOOST_TEST_CHECKPOINT("generating 5 random list elements"); 
  typename T::type a = random<T>(),
                   b = random<T>(),
                   c = random<T>(),
                   d = random<T>(),
                   e = random<T>();

  BOOST_TEST_MESSAGE("a: " << a);
  BOOST_TEST_MESSAGE("b: " << b);
  BOOST_TEST_MESSAGE("c: " << c);
  BOOST_TEST_MESSAGE("d: " << d);
  BOOST_TEST_MESSAGE("e: " << e);

  BOOST_TEST_CHECKPOINT("inserting elements into list"); 
  l.push_back(a);
  l.push_back(b);
  l.push_back(c);
  l.push_back(d);
  l.push_back(e);

  BOOST_TEST_CHECKPOINT("deep copying list into range"); 
  r.deep_copy(l);

  BOOST_TEST_CHECKPOINT("retrieving range iterators"); 
  typename range_type::iterator it = r.begin(), end = r.end();

  BOOST_TEST_CHECKPOINT("verifying list data");
  BOOST_CHECK(*(--end) == e);
  BOOST_CHECK(*(--end) == d);
  BOOST_CHECK(*(--end) == c);
  BOOST_CHECK(*(--end) == b);
  BOOST_CHECK(*(--end) == a);
  BOOST_CHECK(it == end);

  BOOST_TEST_CHECKPOINT("freeing range");
  r.free();
}
 
BOOST_AUTO_TEST_CASE_TEMPLATE(deep_copy_c_str, T, test::strings) {
  typedef range<char const*> range_type;

  range_type ra, rb, rc, rd, re;

  BOOST_TEST_CHECKPOINT("default constructing 5 ranges");
  ra.default_construct();
  rb.default_construct();
  rc.default_construct();
  rd.default_construct();
  re.default_construct();

  BOOST_TEST_CHECKPOINT("generating 5 random c-strings");
  std::string ia = random<T>(),
              ib = random<T>(),
              ic = random<T>(),
              id = random<T>(),
              ie = random<T>();

  char const* a = ia.c_str();
  char const* b = ib.c_str();
  char const* c = ic.c_str();
  char const* d = id.c_str();
  char const* e = ie.c_str();

  BOOST_TEST_MESSAGE("a: " << a);
  BOOST_TEST_MESSAGE("b: " << b);
  BOOST_TEST_MESSAGE("c: " << c);
  BOOST_TEST_MESSAGE("d: " << d);
  BOOST_TEST_MESSAGE("e: " << e);

  BOOST_TEST_CHECKPOINT("deep copying c-strings into ranges");
  ra.deep_copy(a, a + std::strlen(a));
  rb.deep_copy(b, b + std::strlen(b));
  rc.deep_copy(c, c + std::strlen(c));
  rd.deep_copy(d, d + std::strlen(d));
  re.deep_copy(e, e + std::strlen(e));

  BOOST_TEST_CHECKPOINT("forming std::strings from ranges");
  std::string sa(ra.begin(), ra.end()),
              sb(rb.begin(), rb.end()),
              sc(rc.begin(), rc.end()),
              sd(rd.begin(), rd.end()),
              se(re.begin(), re.end());

  BOOST_TEST_CHECKPOINT("freeing ranges");  
  ra.free();
  rb.free();
  rc.free();
  rd.free();
  re.free();

  BOOST_TEST_CHECKPOINT("verifying string data");
  BOOST_CHECK_EQUAL(sa, a);
  BOOST_CHECK_EQUAL(sb, b);
  BOOST_CHECK_EQUAL(sc, c);
  BOOST_CHECK_EQUAL(sd, d);
  BOOST_CHECK_EQUAL(se, e);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(deep_copy_range, T, test::rounds) {
  typedef std::list<typename T::type> list_type;
  typedef range<typename list_type::const_iterator> range_type; 

  list_type l;
  range_type ra, rb;  

  BOOST_TEST_CHECKPOINT("default constructing ranges");
  ra.default_construct();
  rb.default_construct();

  BOOST_TEST_CHECKPOINT("generating 5 random list elements"); 
  typename T::type a = random<T>(),
                   b = random<T>(),
                   c = random<T>(),
                   d = random<T>(),
                   e = random<T>();

  BOOST_TEST_MESSAGE("a: " << a);
  BOOST_TEST_MESSAGE("b: " << b);
  BOOST_TEST_MESSAGE("c: " << c);
  BOOST_TEST_MESSAGE("d: " << d);
  BOOST_TEST_MESSAGE("e: " << e);

  BOOST_TEST_CHECKPOINT("inserting elements into list"); 
  l.push_back(a);
  l.push_back(b);
  l.push_back(c);
  l.push_back(d);
  l.push_back(e);

  BOOST_TEST_CHECKPOINT("deep copying list into first range");
  ra.deep_copy(l);

  BOOST_TEST_CHECKPOINT("deep copying first range into second range");
  rb.deep_copy(ra);

  BOOST_TEST_CHECKPOINT("freeing first range");
  ra.free();

  BOOST_TEST_CHECKPOINT("retrieving second range iterators"); 
  typename range_type::iterator it = rb.begin(), end = rb.end();

  BOOST_TEST_CHECKPOINT("verifying list data");
  BOOST_CHECK(*(--end) == e);
  BOOST_CHECK(*(--end) == d);
  BOOST_CHECK(*(--end) == c);
  BOOST_CHECK(*(--end) == b);
  BOOST_CHECK(*(--end) == a);
  BOOST_CHECK(it == end);

  BOOST_TEST_CHECKPOINT("freeing range");
  rb.free();
}

BOOST_AUTO_TEST_CASE_TEMPLATE(get_to_vector, T, test::rounds) {
  typedef std::vector<typename T::type> vector_type;
  typedef range<typename vector_type::const_iterator> range_type;

  vector_type va;
  range_type r;

  BOOST_TEST_CHECKPOINT("default constructing range");
  r.default_construct();

  BOOST_TEST_CHECKPOINT("generating 5 random vector elements"); 
  typename T::type a = random<T>(),
                   b = random<T>(),
                   c = random<T>(),
                   d = random<T>(),
                   e = random<T>();

  BOOST_TEST_MESSAGE("a: " << a);
  BOOST_TEST_MESSAGE("b: " << b);
  BOOST_TEST_MESSAGE("c: " << c);
  BOOST_TEST_MESSAGE("d: " << d);
  BOOST_TEST_MESSAGE("e: " << e);

  BOOST_TEST_CHECKPOINT("inserting elements into vector"); 
  va.push_back(a);
  va.push_back(b);
  va.push_back(c);
  va.push_back(d);
  va.push_back(e);

  BOOST_TEST_CHECKPOINT("deep copying vector into range");
  r.deep_copy(va);
  
  BOOST_TEST_CHECKPOINT("getting vector from range");
  vector_type vb = r.template get<vector_type>();
  
  BOOST_TEST_CHECKPOINT("freeing range");
  r.free();

  BOOST_TEST_CHECKPOINT("retrieving vector iterators");
  typename vector_type::iterator it = vb.begin(), end = vb.end();

  BOOST_TEST_CHECKPOINT("verifying list data");
  BOOST_CHECK(*(--end) == e);
  BOOST_CHECK(*(--end) == d);
  BOOST_CHECK(*(--end) == c);
  BOOST_CHECK(*(--end) == b);
  BOOST_CHECK(*(--end) == a);
  BOOST_CHECK(it == end);
}

BOOST_AUTO_TEST_SUITE_END()
