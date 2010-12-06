/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#include <iostream>

#include <boost/detail/lightweight_test.hpp>

#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/sexpr/core.hpp>
#include <boost/spirit/home/prana/sexpr/routine/set.hpp>

int main (void) {
  using namespace boost;
  using namespace boost::spirit;

  using prana::sexpr;

  { //[string_literal
    sexpr s;
    
    prana::set(s, "foo");

    BOOST_TEST(s.type == sexpr::ascii::value);
    BOOST_TEST(std::string("foo") ==
               std::string(
                 prana::value_at<2>(*prana::cast<sexpr::ascii>(s)),
                 prana::value_at<2>(*prana::cast<sexpr::ascii>(s)) +
                 prana::value_at<0>(*prana::cast<sexpr::ascii>(s))
               ));
    //]
  }
  
  { //[integer_literal
    sexpr s;
    
    prana::set(s, 5);

    BOOST_TEST(s.type == sexpr::integer::value);
    BOOST_TEST(*prana::cast<sexpr::integer>(s) == 5);
    //]
  }
  
  return boost::report_errors();
}
 
