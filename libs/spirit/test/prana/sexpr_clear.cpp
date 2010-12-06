/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#include <boost/detail/lightweight_test.hpp>

#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/sexpr/core.hpp>
#include <boost/spirit/home/prana/sexpr/routine/clear.hpp>

int main (void) {
  using namespace boost;
  using namespace boost::spirit;

  using prana::sexpr;

  { //[boolean
    sexpr s;

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST((s.data[0] == 0) && (s.data[1] == 0));

    s.type = sexpr::boolean::value;
    *prana::cast<sexpr::boolean>(s) = true;

    prana::clear(s);

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST((s.data[0] == 0) && (s.data[1] == 0));
    //]
  }

  { //[integer
    sexpr s;

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST((s.data[0] == 0) && (s.data[1] == 0));

    s.type = sexpr::integer::value;
    *prana::cast<sexpr::integer>(s) = 42;

    prana::clear(s);

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST((s.data[0] == 0) && (s.data[1] == 0));
    //]
  }

  { //[floating
    sexpr s;

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST((s.data[0] == 0) && (s.data[1] == 0));

    s.type = sexpr::floating::value;
    *prana::cast<sexpr::floating>(s) = 17.5;

    prana::clear(s);

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST((s.data[0] == 0) && (s.data[1] == 0));
    //]
  }

  return boost::report_errors();
}
 
