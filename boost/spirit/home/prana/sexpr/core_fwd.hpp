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

    ((boolean)  ((bool))     (arithmetic) (stack))
    ((integer)  ((intmax_t)) (arithmetic) (stack))
    ((floating) ((double))   (arithmetic) (stack))

    ((nil) ((nil_type)) (sentinel) (stack))

    ((pair)  ((tuple2<sexpr*, sexpr*>)) (cons) (heap))
    ((range) ((tuple2<sexpr*, sexpr*>)) (cons) (stack))
    ((list)  ((tuple2<sexpr*, sexpr*>)) (cons) (heap))

    ((vector) ((tuple3<uinthalf_t, uinthalf_t, sexpr*>)) (dynamic_array) (heap))
    ((ascii)  ((tuple3<uinthalf_t, uinthalf_t, char*>))  (dynamic_array) (heap))
    ((symbol) ((tuple3<uinthalf_t, uinthalf_t, char*>))  (dynamic_array) (heap))
    //]
  )

  typedef basic_registry<type_definitions> registry;

  typedef uintmax_t typeinfo; /*< sizeof(void*) bytes of storage for
                                  typeinfo. >*/

  typeinfo type;
  void*    data [2];

  sexpr (void); /*< Default ctor sets sexpr to nil type. >*/

  ~sexpr (void); /*< Forwards to clear. >*/

  //[set_forwarders
  template<class Value>
    sexpr (Value const&);
  template<class Value>
    sexpr& operator= (Value const&);
  template<class Value>
    void assign (Value const&);
  //]

  //[sexpr_list_interface
  sexpr (tuple2<sexpr*, sexpr*> const&);
  
  void assign (tuple2<sexpr*, sexpr*> const&);
  //]

  //[sexpr_pair_interface
  template<class Car, class Cdr>
    sexpr (Car, Cdr);

  template<class Car, class Cdr>
    void assign (Car, Cdr);
  //]
};
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_CORE_FWD_HPP ->*/

