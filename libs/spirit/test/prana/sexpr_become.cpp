/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#include <boost/detail/lightweight_test.hpp>

#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/support/no_op.hpp>
#include <boost/spirit/home/prana/adt/function_node.hpp>
#include <boost/spirit/home/prana/sexpr/core.hpp>
#include <boost/spirit/home/prana/sexpr/procedure/become.hpp>

int main (void) {
  using namespace boost;
  using namespace boost::spirit;

  using prana::sexpr;

  typedef prana::scope<sexpr> scope_type;
  typedef prana::unary_no_op_with_return<sexpr, scope_type> no_op_type;
  typedef prana::stored_function<sexpr, no_op_type> function_type;

  { //[boolean_to_boolean
    sexpr s;

    s.type = sexpr::boolean::value;
    *prana::cast_car<sexpr::boolean>(s) = true;

    prana::become<sexpr::boolean>(s);

    BOOST_TEST(s.type == sexpr::boolean::value);
    BOOST_TEST(*prana::cast_car<sexpr::boolean>(s) == true);
    //]
  }
  
  { //[ascii_char_to_integer
    sexpr s;

    s.type = sexpr::ascii_char::value;
    *prana::cast_car<sexpr::ascii_char>(s) = 'x';

    prana::become<sexpr::integer>(s);

    BOOST_TEST(s.type == sexpr::integer::value);
    BOOST_TEST(s.car == 0);
    //]
  }
  
  { //[function_to_floating
    sexpr s;

    s.type = sexpr::function::value;
    *prana::cast_car<sexpr::function>(s) = new function_type();

    prana::become<sexpr::floating>(s); /*< Implicit clear. >*/

    BOOST_TEST(s.type == sexpr::floating::value);
    BOOST_TEST(s.car == 0);
    //]
  }

  return boost::report_errors();
}
 
