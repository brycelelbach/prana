/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>

int main (void) {
  using namespace boost::spirit::prana;

  BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(
    //[arithmetic_types
    ((sizeof(schematic::boolean::data_type))  (sizeof(void*[2])))
    ((sizeof(schematic::integer::data_type))  (sizeof(void*[2])))
    ((sizeof(schematic::floating::data_type)) (sizeof(void*[2])))
    //]

    //[sentinel_types
    ((sizeof(schematic::sentinel::data_type)) (sizeof(void*[2])))
    //]

    //[cons_types
    ((sizeof(schematic::pair::data_type))  (sizeof(void*[2])))
    ((sizeof(schematic::range::data_type)) (sizeof(void*[2])))
    ((sizeof(schematic::list::data_type))  (sizeof(void*[2])))
    //]

    //[vector_types
    ((sizeof(schematic::vector::data_type)) (sizeof(void*[2])))
    ((sizeof(schematic::ascii::data_type))  (sizeof(void*[2])))
    ((sizeof(schematic::symbol::data_type)) (sizeof(void*[2]))))
    //]

  return boost::report_errors();
}
 
