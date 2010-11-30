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

//[ sexpr_traits
BOOST_SPIRIT_PRANA_DEFINE_TRAITS(
  false,
  ((arithmetic_type) (is_arithmetic_type)) /*< These types represent numeric
                                               data, e.g. integers, floating
                                               point types, etc. >*/ 
  ((sentinel_type)   (is_sentinel_type))   /*< Sentinel types are singletons
                                               used to terminate recursive
                                               s-expressions. >*/
  ((symbol_type)     (is_symbol_type))     /*< Atoms representing language
                                               symbols. >*/
  ((function_type)   (is_function_type))   /*< Functions are represented in
                                               Prana by C++ functors. >*/
  ((text_type)       (is_text_type))       /*< Atoms representing textual
                                               data. >*/
  ((container_type)  (is_container_type))) /*< Abstract data structures. >*/
//]

//[ is_stack_allocated_type
template<class T>
struct is_stack_allocated_type:
  mpl::or_<
    is_arithmetic_type<T>,
    is_sentinel_type<T>,
    is_symbol_type<T>
  > { };
//]

//[ is_heap_allocated_type
template<class T>
struct is_heap_allocated_type:
  mpl::or_<
    is_function_type<T>,
    is_text_type<T>,
    is_container_type<T>
  > { };
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_TRAITS_HPP ->*/

