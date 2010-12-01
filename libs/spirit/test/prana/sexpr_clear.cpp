/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#include <boost/detail/lightweight_test.hpp>

#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/support/no_op.hpp>
#include <boost/spirit/home/prana/adt/function_node.hpp>
#include <boost/spirit/home/prana/sexpr/clear.hpp>
#include <boost/spirit/home/prana/sexpr/core.hpp>

int main (void) {
  using namespace boost;
  using namespace boost::spirit;

  typedef std::string::iterator iterator;
  typedef prana::sexpr<iterator> sexpr_type;
  typedef prana::scope<sexpr_type> scope_type;
  typedef prana::unary_no_op_with_return<sexpr_type, scope_type> no_op_type;
  typedef prana::stored_function<sexpr_type, no_op_type> function_type;
  typedef prana::symbol_table<iterator> symbol_table_type;

  { //[boolean
    sexpr_type s;

    BOOST_TEST(s.type == sexpr_type::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);

    s.type = sexpr_type::boolean::value;
    *prana::cast_car<sexpr_type::boolean>(s) = true;

    prana::clear(s);

    BOOST_TEST(s.type == sexpr_type::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);
    //]
  }

  { //[integer
    sexpr_type s;

    BOOST_TEST(s.type == sexpr_type::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);

    s.type = sexpr_type::integer::value;
    *prana::cast_car<sexpr_type::integer>(s) = 42;

    prana::clear(s);

    BOOST_TEST(s.type == sexpr_type::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);
    //]
  }

  { //[floating
    sexpr_type s;

    BOOST_TEST(s.type == sexpr_type::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);

    s.type = sexpr_type::floating::value;
    *prana::cast_car<sexpr_type::floating>(s) = 17.5;

    prana::clear(s);

    BOOST_TEST(s.type == sexpr_type::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);
    //]
  }

  { //[symbol
    std::string str = "fizzbuzz";
    symbol_table_type symtab;

    sexpr_type s;

    BOOST_TEST(s.type == sexpr_type::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);

    s.type = sexpr_type::symbol::value;
    *prana::cast_car<sexpr_type::symbol>(s) =
      symtab.insert(str.begin(), str.end(), function_type());

    prana::clear(s);

    BOOST_TEST(s.type == sexpr_type::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);
    //]
  }

  { //[function
    sexpr_type s;

    BOOST_TEST(s.type == sexpr_type::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);

    s.type = sexpr_type::function::value;
    *prana::cast_car<sexpr_type::function>(s) = new function_type();

    prana::clear(s);

    BOOST_TEST(s.type == sexpr_type::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);
    //]
  }
  
  { //[ascii
    std::string str = "foobar";

    sexpr_type s;

    BOOST_TEST(s.type == sexpr_type::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);

    s.type = sexpr_type::ascii::value;
    *prana::cast_car<sexpr_type::ascii>(s) = 
      new boost::iterator_range<std::string::iterator>(str.begin(), str.end());

    prana::clear(s);

    BOOST_TEST(s.type == sexpr_type::nil::value);
    BOOST_TEST(s.car == 0);
    BOOST_TEST(s.cdr == 0);
    //]
  }

  return boost::report_errors();
}
 
