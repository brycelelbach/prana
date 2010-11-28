/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_CORE_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_CORE_HPP

#include <boost/config.hpp>
#include <boost/cstdint.hpp>

#include <boost/mpl/or.hpp>

#include <boost/range/iterator_range.hpp>

#include <boost/spirit/home/prana/traits.hpp>
#include <boost/spirit/home/prana/registry.hpp>
#include <boost/spirit/home/prana/adt/symbol_table.hpp>
#include <boost/spirit/home/prana/adt/function_node.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[ sexpr_traits
BOOST_SPIRIT_PRANA_DEFINE_TRAITS(
  false,
  ((arithmetic_type) (is_arithmetic_type)) /*< These types represent numeric
                                               data, e.g. integers, floating
                                               point types, etc. >*/ 
  ((sentinel_type)   (is_sentinel_type)) /*< Sentinel types are singletons used
                                             to terminate recursive
                                             s-expressions. >*/
  ((function_type)   (is_function_type)) /*< Functions are represented in Prana
                                             by C++ functors. >*/
  ((text_type)       (is_text_type)) /*< Atoms representing textual data. >*/
  ((container_type)  (is_container_type)) /*< Abstract data structures. >*/
//]
)

//[ is_stack_allocated_type
template<class T>
struct is_stack_allocated_type:
  mpl::or_<
    is_arithmetic_type<T>,
    is_sentinel_type<T>
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

//[ sexpr 
template<class Iterator>
struct sexpr {
  struct universal_tree; /*< Indicates that sexpr fulfills
                             UniversalTree. >*/

  typedef typename symbol_table<Iterator, sexpr*>::pointer symbol_type;

  BOOST_SPIRIT_PRANA_REGISTRY_SET( /*< Define the s-expression Boost.MPL
                                       registry set. >*/ 
    std::size_t, core_types,

    //[ sexpr_arithmetic_types
    ((boolean)  (bool)            (arithmetic_type))
    ((integer)  (boost::intmax_t) (arithmetic_type))
    ((floating) (double)          (arithmetic_type))
    //]

    //[ sexpr_sentinel_types
    ((nil)        (nil_type*)        (sentinel_type))
    ((empty_list) (empty_list_type*) (sentinel_type))
    //]

    //[ sepxr_function_types
    ((function) (function_node<sexpr>*) (function_type))
    //]

    //[ sexpr_text_types
    ((symbol) (symbol_type)               (text_type))
    ((string) (iterator_range<Iterator>*) (text_type))
    //]

    //[ sexpr_container_types
    ((pair)          (sexpr*) (container_type))
    ((proper_list)   (sexpr*) (container_type))
    ((circular_list) (sexpr*) (container_type))
    //]
  )

  typedef basic_registry<core_types> registry;

  typedef boost::uintmax_t typeinfo;

  typeinfo type;
  void*    car;
  sexpr*   cdr;
};
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_CORE_HPP ->*/

