/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCHEMATIC_CORE_FWD_HPP)
#define BOOST_SPIRIT_PRANA_SCHEMATIC_CORE_FWD_HPP

#include <boost/config.hpp>
#include <boost/cstdint.hpp>

#include <boost/spirit/home/prana/dispatch/registry.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/adt/tuple.hpp>
#include <boost/spirit/home/prana/support/half_t.hpp>
#include <boost/spirit/home/prana/traits_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[schematic
struct schematic {
  struct visitable; /*< Indicates that schematic fulfills Visitable. >*/

  typedef std::size_t size_type;

  BOOST_SPIRIT_PRANA_REGISTRY_SET( /*< Define the s-expression RegistrySet,
                                       which is an MPL set. >*/
    size_type,        /*< The value_type for each TypeDefinition. >*/
    type_definitions, /*< The name for the RegistrySet. >*/

    ((boolean)  ((bool))     (arithmetic) (stack))
    ((integer)  ((intmax_t)) (arithmetic) (stack))
    ((floating) ((double))   (arithmetic) (stack))

    ((sentinel) ((sentinel_type)) (singleton) (stack))

    ((pair)  ((tuple2<schematic*, schematic*>)) (cons) (heap))
    ((range) ((tuple2<schematic*, schematic*>)) (cons) (stack))
    ((list)  ((tuple2<schematic*, schematic*>)) (cons) (heap))

    ((vector) ((tuple3<uinthalf_t, uinthalf_t, schematic*>))
      (dynamic_array) (heap))
    ((ascii)  ((tuple3<uinthalf_t, uinthalf_t, char*>))
      (dynamic_array) (heap))
    ((symbol) ((tuple3<uinthalf_t, uinthalf_t, char*>))
      (dynamic_array) (heap))
    //]
  )

  typedef basic_registry<type_definitions> registry;

  typedef uintmax_t typeinfo; /*< sizeof(void*) bytes of storage for
                                  typeinfo. >*/

  typeinfo type;
  void*    data [2];

  schematic (void); /*< Default ctor sets schematic to sentinel type. >*/

  ~schematic (void); /*< Forwards to clear. >*/

  //[set_forwarders
  template<class Value>
    schematic (Value const&);
  template<class Value>
    schematic& operator= (Value const&);
  template<class Value>
    void assign (Value const&);
  //]

  //[schematic_list_interface
  schematic (tuple2<schematic*, schematic*> const&);
  
  void assign (tuple2<schematic*, schematic*> const&);
  //]

  //[schematic_pair_interface
  template<class Car, class Cdr>
    schematic (Car, Cdr);

  template<class Car, class Cdr>
    void assign (Car, Cdr);
  //]
};
//]

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCHEMATIC_CORE_FWD_HPP

