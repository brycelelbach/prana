/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_TRAITS_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_TRAITS_HPP

#include <boost/config.hpp>

#include <boost/mpl/or.hpp>

#include <boost/spirit/home/prana/traits.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[sexpr_traits
BOOST_SPIRIT_PRANA_TRAITS(
  false,
  // Atoms.
  ((arithmetic_type) (is_arithmetic_type)) /*< These types represent numeric
                                               data, e.g. integers, floating
                                               point types, etc. >*/ 
  ((sentinel_type)   (is_sentinel_type))   /*< Sentinel types are singletons
                                               used to terminate recursive
                                               s-expressions. >*/

  // Abstract data structures.
  ((vector_type) (is_vector_type))
  ((cons_type)   (is_cons_type))       

  // Storage details.
  ((heap)  (is_heap_allocated_type))   
  ((stack) (is_stack_allocated_type))) 
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_TRAITS_HPP ->*/

