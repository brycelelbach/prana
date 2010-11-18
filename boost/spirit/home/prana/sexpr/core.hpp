/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_CORE_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_CORE_HPP

#include <boost/config.hpp>
#include <boost/cstdint.hpp>

#include <boost/range/iterator_range.hpp>

#include <boost/spirit/home/prana/registry.hpp>
#include <boost/spirit/home/prana/adt/intern_pool.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Iterator>
struct sexpr {
  BOOST_SPIRIT_PRANA_TYPES(
    std::size_t,
    ((boolean)  (bool))
    ((integer)  (boost::intmax_t))
    ((floating) (double))
    ((pointer)  (sexpr*))
    ((symbol)   (typename intern_pool<Iterator>::pointer))
    ((string)   (typename intern_pool<Iterator>::pointer))
    ((nil)      (void*))
    ((empty)    (void*))
    ((cons)     (sexpr*))
    ((tuple)    (sexpr*)))

  BOOST_SPIRIT_PRANA_TEMP_REGISTRY_SET(
    numeric_types, 
    (boolean)(integer)(floating)(pointer))

  BOOST_SPIRIT_PRANA_TEMP_EXTEND_REGISTRY_SET(
    pod_types, string_types, numeric_types,
    (symbol)(string))
  
  BOOST_SPIRIT_PRANA_TEMP_EXTEND_REGISTRY_SET(
    atom_types, special_types, pod_types, 
    (nil)(empty))

  BOOST_SPIRIT_PRANA_TEMP_EXTEND_REGISTRY_SET(
    core_types, recursive_types, atom_types,
    (cons)(tuple))

  typedef basic_registry<core_types> registry;

  typedef boost::uintmax_t typeinfo;

  typeinfo type;
  void*    car;
  sexpr*   cdr;
};

BOOST_SPIRIT_PRANA_TEMP_INIT_TYPES(
  sexpr, (Iterator), core_types, 
  (nil)
  (empty)
  (boolean)
  (integer)
  (floating)
  (symbol)
  (string)
  (pointer)
  (cons)
  (tuple))

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SEXPR_CORE_HPP
