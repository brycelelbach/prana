/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE utree_basic_ctors

#include <boost/spirit/home/prana/utree.hpp>
#include <boost/test/unit_test.hpp>

using namespace boost::spirit::prana;

BOOST_AUTO_TEST_CASE(bool_ctor) {
  utree t0(true);
  utree t1(false);
}

BOOST_AUTO_TEST_CASE(char_ctor) {
  char c = 'f';

  utree t0('b');
  utree t1('d');
  utree t2('x');
  utree t3(c);
}

BOOST_AUTO_TEST_CASE(unsigned_int_ctor) {
  unsigned int i = 79;
  
  utree t0(142);
  utree t1(17);
  utree t2(4215);
  utree t3(i);
}

BOOST_AUTO_TEST_CASE(signed_int_ctor) {
  signed int i = -53;
  
  utree t0(14);
  utree t1(3023);
  utree t2(-105);
  utree t3(i);
}

BOOST_AUTO_TEST_CASE(double_ctor) {
  double d = 531.53;
  
  utree t0(6315.353);
  utree t1(33.91);
  utree t2(0.5389);
  utree t3(d);
}

BOOST_AUTO_TEST_CASE(str_ctor) {
  char const* cs = "hello world";
  std::string ss = "hello world";
  
  utree t0("foo");
  utree t1("bar");
  utree t2("foobar");
  utree t3(cs);
  utree t4(cs, 5);
  utree t5("the long string is quite long, and might break fast_string<>");
  utree t6(ss);
  utree t7(std::string("T am temporary!"));
}