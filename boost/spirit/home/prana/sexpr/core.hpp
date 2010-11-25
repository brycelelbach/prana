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
#include <boost/spirit/home/prana/adt/symbol_table.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Iterator>
struct sexpr {
  struct symbolic_expression; /* indicates sexpr fulfills SymbolicExpression */

  BOOST_SPIRIT_PRANA_TYPES(
    std::size_t, core_types,
    ((boolean)        (bool))
    ((integer)        (boost::intmax_t))
    ((floating)       (double))
    ((pointer)        (sexpr*))
    ((symbol)         (iterator_range<Iterator>*))
    ((string)         (iterator_range<Iterator>*))
    ((nil)            (void*))
    ((empty)          (void*))
    ((pair)           (sexpr*))
    ((proper_list)    (sexpr*))
    ((circular_list)  (sexpr*)))

  typedef basic_registry<core_types> registry;

  typedef boost::uintmax_t typeinfo;

  typeinfo type;
  void*    car;
  sexpr*   cdr;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SEXPR_CORE_HPP
