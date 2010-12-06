/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#include <boost/detail/lightweight_test.hpp>

#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/sexpr/core.hpp>
#include <boost/spirit/home/prana/sexpr/routine/become.hpp>

int main (void) {
  using namespace boost;
  using namespace boost::spirit;

  using prana::sexpr;

  { //[boolean_to_boolean
    sexpr s;

    s.type = sexpr::boolean::value;
    *prana::cast<sexpr::boolean>(s) = true;

    prana::become<sexpr::boolean>(s); /*< Implicit clear. >*/

    BOOST_TEST(s.type == sexpr::boolean::value);
    BOOST_TEST((s.data[0] == 0) && (s.data[1] == 0));
    //]
  }
  
  { //[floating_to_integer
    sexpr s;

    s.type = sexpr::floating::value;
    *prana::cast<sexpr::floating>(s) = 21.53;

    prana::become<sexpr::integer>(s);

    BOOST_TEST(s.type == sexpr::integer::value);
    BOOST_TEST((s.data[0] == 0) && (s.data[1] == 0));
    //]
  }

  return boost::report_errors();
}
 
