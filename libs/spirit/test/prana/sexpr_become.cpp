/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#include <boost/detail/lightweight_test.hpp>

#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/support/no_op.hpp>
#include <boost/spirit/home/prana/adt/function_node.hpp>
#include <boost/spirit/home/prana/sexpr/become.hpp>
#include <boost/spirit/home/prana/sexpr/core.hpp>

int main (void) {
  using namespace boost;
  using namespace boost::spirit;

  typedef std::string::iterator iterator;
  typedef prana::sexpr<iterator> sexpr_type;
  typedef prana::scope<sexpr_type> scope_type;
  typedef prana::unary_no_op_with_return<sexpr_type, scope_type> no_op_type;
  typedef prana::stored_function<sexpr_type, no_op_type> function_type;

  { //[boolean_to_boolean
    sexpr_type s;

    s.type = sexpr_type::boolean::value;
    *prana::cast_car<sexpr_type::boolean>(s) = true;

    prana::become<sexpr_type::boolean>(s);

    BOOST_TEST(s.type == sexpr_type::boolean::value);
    BOOST_TEST(*prana::cast_car<sexpr_type::boolean>(s) == true);
    //]
  }
  
  { //[boolean_to_integer
    sexpr_type s;

    s.type = sexpr_type::boolean::value;
    *prana::cast_car<sexpr_type::boolean>(s) = true;

    prana::become<sexpr_type::integer>(s);

    BOOST_TEST(s.type == sexpr_type::integer::value);
    BOOST_TEST(s.car == 0);
    //]
  }
  
  { //[function_to_floating
    sexpr_type s;

    s.type = sexpr_type::function::value;
    *prana::cast_car<sexpr_type::function>(s) = new function_type();

    prana::become<sexpr_type::floating>(s); /*< Implicit clear. >*/

    BOOST_TEST(s.type == sexpr_type::floating::value);
    BOOST_TEST(s.car == 0);
    //]
  }

  return boost::report_errors();
}
 
