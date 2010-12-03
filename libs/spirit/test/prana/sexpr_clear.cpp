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
#include <boost/spirit/home/prana/sexpr/procedure/clear.hpp>

int main (void) {
  using namespace boost;
  using namespace boost::spirit;

  using prana::sexpr;

  typedef prana::scope<sexpr> scope_type;
  typedef prana::unary_no_op_with_return<sexpr, scope_type> no_op_type;
  typedef prana::stored_function<sexpr, no_op_type> function_type;
  typedef prana::symbol_table<char> symbol_table_type;

  { //[boolean
    sexpr s;

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);

    s.type = sexpr::boolean::value;
    *prana::cast_car<sexpr::boolean>(s) = true;

    prana::clear(s);

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);
    //]
  }

  { //[integer
    sexpr s;

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);

    s.type = sexpr::integer::value;
    *prana::cast_car<sexpr::integer>(s) = 42;

    prana::clear(s);

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);
    //]
  }

  { //[floating
    sexpr s;

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);

    s.type = sexpr::floating::value;
    *prana::cast_car<sexpr::floating>(s) = 17.5;

    prana::clear(s);

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);
    //]
  }

  { //[symbol
    std::string str = "fizzbuzz";
    symbol_table_type symtab;

    sexpr s;

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);

    s.type = sexpr::symbol::value;
    *prana::cast_car<sexpr::symbol>(s) =
      symtab.insert(str.begin(), str.end(), function_type());

    prana::clear(s);

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);
    //]
  }

  { //[function
    sexpr s;

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);

    s.type = sexpr::function::value;
    *prana::cast_car<sexpr::function>(s) = new function_type();

    prana::clear(s);

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);
    //]
  }
  
  { //[ascii_string
    sexpr s;

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);

    s.type = sexpr::ascii_string::value;
    *prana::cast_car<sexpr::ascii_string>(s) = new std::string("foobar"); 

    prana::clear(s);

    BOOST_TEST(s.type == sexpr::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);
    //]
  }

  return boost::report_errors();
}
 
