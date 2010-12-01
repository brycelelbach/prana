/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_CORE_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_CORE_HPP

#include <boost/config.hpp>
#include <boost/cstdint.hpp>

#include <boost/range/iterator_range.hpp>

#include <boost/spirit/home/prana/registry.hpp>
#include <boost/spirit/home/prana/support/fltptr_t.hpp>
#include <boost/spirit/home/prana/adt/symbol_table.hpp>
#include <boost/spirit/home/prana/adt/function_node.hpp>
#include <boost/spirit/home/prana/sexpr/traits.hpp>
#include <boost/spirit/home/prana/sexpr/clear.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[sexpr
template<class Iterator>
struct sexpr {
  struct universal_tree; /*< Indicates that sexpr fulfills UniversalTree. >*/

  typedef std::size_t size_type;

  BOOST_SPIRIT_PRANA_REGISTRY_SET( /*< Define the s-expression RegistrySet,
                                       which is an MPL set. >*/
    size_type,        /*< The value_type for each TypeDefinition. >*/
    type_definitions, /*< The name for the RegistrySet. >*/

    ((boolean)  (bool)     (arithmetic_type))
    ((integer)  (intmax_t) (arithmetic_type))
    ((floating) (fltptr_t) (arithmetic_type))

    ((nil)        (nil_type)        (sentinel_type))
    ((empty_list) (empty_list_type) (sentinel_type))

    ((symbol) (typename symbol_table<Iterator>::pointer) (symbol_type))

    ((ascii) (iterator_range<Iterator>*)  (text_type))
    ((utf8) (iterator_range<Iterator>*)   (text_type))
    ((binary) (iterator_range<Iterator>*) (text_type))
    
    ((function) (function_node<sexpr>*) (function_type))

    ((pair)          (sexpr*) (container_type))
    ((proper_list)   (sexpr*) (container_type))
    ((circular_list) (sexpr*) (container_type))
    ((vector)        (sexpr*) (container_type))
    //]
  )

  typedef basic_registry<type_definitions> registry; /*< Define the
                                                         TypeRegistry, using
                                                         basic_registry. >*/

  typedef boost::uintmax_t typeinfo; /*< sizeof(void*) bytes of storage for
                                         typeinfo. >*/

  typeinfo type;
  void*    car;
  sexpr*   cdr;

  sexpr (void);

  ~sexpr (void);
};
//]

template<class Iterator>
sexpr<Iterator>::sexpr (void):
  type(nil::value), car(0), cdr(0) { }

template<class Iterator>
sexpr<Iterator>::~sexpr (void) {
  clear(*this);
}

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_CORE_HPP ->*/

