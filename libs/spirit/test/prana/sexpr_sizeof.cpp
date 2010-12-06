
/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#include <boost/detail/lightweight_test.hpp>

#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/sexpr/core.hpp>

int main (void) {
  using namespace boost;
  using namespace boost::spirit;

  using prana::sexpr;

  BOOST_TEST(sizeof(sexpr::boolean::data_type) <= sizeof(void*[2]));
  BOOST_TEST(sizeof(sexpr::integer::data_type) <= sizeof(void*[2]));
  BOOST_TEST(sizeof(sexpr::floating::data_type) <= sizeof(void*[2]));
  BOOST_TEST(sizeof(sexpr::nil::data_type) <= sizeof(void*[2]));
  BOOST_TEST(sizeof(sexpr::pair::data_type) <= sizeof(void*[2]));
  BOOST_TEST(sizeof(sexpr::range::data_type) <= sizeof(void*[2]));
  BOOST_TEST(sizeof(sexpr::list::data_type) <= sizeof(void*[2]));
  BOOST_TEST(sizeof(sexpr::list::data_type) <= sizeof(void*[2]));
  BOOST_TEST(sizeof(sexpr::vector::data_type) <= sizeof(void*[2]));
  BOOST_TEST(sizeof(sexpr::ascii::data_type) <= sizeof(void*[2]));
  BOOST_TEST(sizeof(sexpr::symbol::data_type) <= sizeof(void*[2]));

  return boost::report_errors();
}
 
