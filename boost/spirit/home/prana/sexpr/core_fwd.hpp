/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_CORE_FWD_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_CORE_FWD_HPP

#include <boost/config.hpp>
#include <boost/cstdint.hpp>

#include <boost/spirit/home/prana/registry.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/adt/tuple.hpp>
#include <boost/spirit/home/prana/support/half_t.hpp>
#include <boost/spirit/home/prana/sexpr/traits.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[sexpr
struct sexpr {
  struct universal_tree; /*< Indicates that sexpr fulfills UniversalTree. >*/

  typedef std::size_t size_type;

  BOOST_SPIRIT_PRANA_REGISTRY_SET( /*< Define the s-expression RegistrySet,
                                       which is an MPL set. >*/
    size_type,        /*< The value_type for each TypeDefinition. >*/
    type_definitions, /*< The name for the RegistrySet. >*/

    ((boolean)  ((bool))     (arithmetic_type) (stack))
    ((integer)  ((intmax_t)) (arithmetic_type) (stack))
    ((floating) ((double))   (arithmetic_type) (stack))

    ((nil) ((nil_type)) (sentinel_type) (stack))

    ((pair)  ((tuple2<sexpr*, sexpr*>)) (cons_type) (heap))
    ((range) ((tuple2<sexpr*, sexpr*>)) (cons_type) (stack))
    ((list)  ((tuple2<sexpr*, sexpr*>)) (cons_type) (heap))

    ((vector) ((tuple3<uinthalf_t, uinthalf_t, sexpr*>)) (vector_type) (heap))
    ((ascii)  ((tuple3<uinthalf_t, uinthalf_t, char*>))  (vector_type) (heap))
    ((symbol) ((tuple3<uinthalf_t, uinthalf_t, char*>))  (vector_type) (heap))
    //]
  )

  typedef basic_registry<type_definitions> registry;

  typedef uintmax_t typeinfo; /*< sizeof(void*) bytes of storage for
                                  typeinfo. >*/

  typeinfo type;
  void*    data [2];

  sexpr (void);

  ~sexpr (void);
};
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_CORE_FWD_HPP ->*/

