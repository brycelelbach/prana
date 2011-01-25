/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/schematic/core.hpp>

int main (void) { try {
  using boost::spirit::prana::schematic;

  BSP_BOOLEAN_TESTS(
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
  
  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}
 
