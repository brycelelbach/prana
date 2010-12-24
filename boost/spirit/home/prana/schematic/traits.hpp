/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCHEMATIC_TRAITS_HPP)
#define BOOST_SPIRIT_PRANA_SCHEMATIC_TRAITS_HPP

#include <boost/config.hpp>

#include <boost/mpl/or.hpp>

#include <boost/spirit/home/prana/traits.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[schematic_traits
BOOST_SPIRIT_PRANA_TRAITS(
  false,
  // Atoms.
  ((arithmetic_type) (is_arithmetic_type)) /*< These types represent numeric
                                               data, e.g. integers, floating
                                               point types, etc. >*/ 
  ((singleton_type)  (is_singleton_type))  /*< Singleton types are used to
                                               terminate recursive s-expressions
                                               and to represent special objects
                                               (e.g. empty and sentinel). >*/

  // Composites.
  ((dynamic_array_type) (is_dynamic_array_type))
  ((cons_type)          (is_cons_type))       

  // Storage details.
  ((heap)  (is_heap_allocated_type))   
  ((stack) (is_stack_allocated_type))) 
//]

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCHEMATIC_TRAITS_HPP

