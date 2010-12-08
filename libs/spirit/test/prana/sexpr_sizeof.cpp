/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#include "harness.hpp"

#include <boost/spirit/home/prana/sexpr/core.hpp>

int main (void) {
  using namespace boost::spirit::prana;

  BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
    //[arithmetic_types
    ((sizeof(sexpr::boolean::data_type))  (sizeof(void*[2])))
    ((sizeof(sexpr::integer::data_type))  (sizeof(void*[2])))
    ((sizeof(sexpr::floating::data_type)) (sizeof(void*[2])))
    //]

    //[sentinel_types
    ((sizeof(sexpr::nil::data_type)) (sizeof(void*[2])))
    //]

    //[cons_types
    ((sizeof(sexpr::pair::data_type))  (sizeof(void*[2])))
    ((sizeof(sexpr::range::data_type)) (sizeof(void*[2])))
    ((sizeof(sexpr::list::data_type))  (sizeof(void*[2])))
    //]

    //[vector_types
    ((sizeof(sexpr::vector::data_type)) (sizeof(void*[2])))
    ((sizeof(sexpr::ascii::data_type))  (sizeof(void*[2])))
    ((sizeof(sexpr::symbol::data_type)) (sizeof(void*[2]))))
    //]

  return boost::report_errors();
}
 
