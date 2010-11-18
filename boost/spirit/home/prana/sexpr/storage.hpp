/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_HPP

#include <boost/config.hpp>
#include <boost/cstdint.hpp>

#include <boost/range/iterator_range.hpp>

#include <boost/spirit/home/prana/registry.hpp>
#include <boost/spirit/home/prana/support/intern_pool.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct sexpr {
  BOOST_SPIRIT_PRANA_TAGS(
    std::size_t,
    ((nil)      (void*))
    ((empty)    (void*))
    ((boolean)  (bool))
    ((integer)  (boost::intmax_t))
    ((floating) (double))
    ((symbol)   (intern_pool<char const*>::iterator))
    ((string)   (iterator_range<std::string::const_iterator>*))
    ((pointer)  (sexpr*))
    ((cons)     (sexpr*))
    ((tuple)    (sexpr*)))

  BOOST_SPIRIT_PRANA_REGISTRY_SET(
    special_objects,
    (nil)(empty))

  BOOST_SPIRIT_PRANA_EXTEND_REGISTRY_SET(
    numeric_tower, special_objects,
    (boolean)(integer)(floating))

  BOOST_SPIRIT_PRANA_EXTEND_REGISTRY_SET(
    string_objects, numeric_tower
    (symbol)(string))

  BOOST_SPIRIT_PRANA_EXTEND_REGISTRY_SET(
    core_types, string_objects,
    (pointer)(cons)(tuple))

  typedef basic_registry<core_types> registry;

  typedef boost::uintmax_t typeinfo;

  typeinfo type;
  void*    car;
  sexpr*   cdr;
};

BOOST_SPIRIT_PRANA_INIT_TAGS(
  sexpr::core_types,
  (sexpr::nil)
  (sexpr::empty)
  (sexpr::boolean)
  (sexpr::integer)
  (sexpr::floating)
  (sexpr::symbol)
  (sexpr::string)
  (sexpr::pointer)
  (sexpr::cons)
  (sexpr::tuple))

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SEXPR_HPP
