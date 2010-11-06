/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_adt_range

#include <boost/spirit/home/prana/adt/range.hpp>

#include <fixture.hpp>

using namespace boost::spirit::prana;
using namespace boost::spirit::prana::adt;

BOOST_FIXTURE_TEST_SUITE(prana_range, test::fixture) 

BOOST_AUTO_TEST_CASE_TEMPLATE(deep_copy_container, T, test::containers) {
  typedef range<typename T::container::const_iterator> range_type;

  typename T::container c;
  range_type r;

  BOOST_TEST_CHECKPOINT("default constructing range");
  r.default_construct();

  BOOST_TEST_CHECKPOINT(
    "creating container with " << T::elements::value << " elements"
  );
  for (std::size_t i = 0; i < T::elements::value; ++i) {
    c.push_back(generator.get<T>());
    BOOST_TEST_MESSAGE("element " << i << ": " << c.back()); 
  }

  BOOST_TEST_CHECKPOINT("deep copying container into range"); 
  r.deep_copy(c);

  BOOST_TEST_CHECKPOINT("retrieving iterators"); 
  typename range_type::iterator rit = r.begin(), rend = r.end();

  BOOST_TEST_CHECKPOINT("verifying retrieved iterators");
  typename T::container::const_iterator cit = c.begin(), cend = c.end();
  for (std::size_t i = 0; i < T::elements::value; ++i) {
    BOOST_CHECK_EQUAL(*cit, *rit); ++cit; ++rit;
  }
 
  BOOST_CHECK(rit == rend);
  BOOST_CHECK(cit == cend);

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
  std::string ia = generator.get<T>(),
              ib = generator.get<T>(),
              ic = generator.get<T>(),
              id = generator.get<T>(),
              ie = generator.get<T>();

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

BOOST_AUTO_TEST_CASE_TEMPLATE(deep_copy_range, T, test::containers) {
  typedef range<typename T::container::const_iterator> range_type; 

  typename T::container c;
  range_type ra, rb;  

  BOOST_TEST_CHECKPOINT("default constructing ranges");
  ra.default_construct();
  rb.default_construct();

  BOOST_TEST_CHECKPOINT(
    "creating container with " << T::elements::value << " elements"
  );
  for (std::size_t i = 0; i < T::elements::value; ++i) {
    c.push_back(generator.get<T>());
    BOOST_TEST_MESSAGE("element " << i << ": " << c.back()); 
  }

  BOOST_TEST_CHECKPOINT("deep copying container into first range");
  ra.deep_copy(c);

  BOOST_TEST_CHECKPOINT("deep copying first range into second range");
  rb.deep_copy(ra);

  BOOST_TEST_CHECKPOINT("freeing first range");
  ra.free();

  BOOST_TEST_CHECKPOINT("retrieving second range iterators"); 
  typename range_type::iterator rit = rb.begin(), rend = rb.end();

  BOOST_TEST_CHECKPOINT("verifying retrieved iterators");
  typename T::container::const_iterator cit = c.begin(), cend = c.end();
  for (std::size_t i = 0; i < T::elements::value; ++i) {
    BOOST_CHECK_EQUAL(*cit, *rit); ++cit; ++rit;
  }
 
  BOOST_CHECK(rit == rend);
  BOOST_CHECK(cit == cend);

  BOOST_TEST_CHECKPOINT("freeing range");
  rb.free();
}

BOOST_AUTO_TEST_CASE_TEMPLATE(get_to_container, T, test::containers) {
  typedef range<typename T::container::const_iterator> range_type;

  typename T::container ca;
  range_type r;

  BOOST_TEST_CHECKPOINT("default constructing range");
  r.default_construct();

  BOOST_TEST_CHECKPOINT(
    "creating container with " << T::elements::value << " elements"
  );
  for (std::size_t i = 0; i < T::elements::value; ++i) {
    ca.push_back(generator.get<T>());
    BOOST_TEST_MESSAGE("element " << i << ": " << ca.back()); 
  }

  BOOST_TEST_CHECKPOINT("deep copying container into range"); 
  r.deep_copy(ca);
  
  BOOST_TEST_CHECKPOINT("getting container from range");
  typename T::container cb = r.template get<typename T::container>();
  
  BOOST_TEST_CHECKPOINT("freeing range");
  r.free();

  BOOST_TEST_CHECKPOINT("verifying container data");
  BOOST_CHECK(ca == cb);
}

BOOST_AUTO_TEST_SUITE_END()
