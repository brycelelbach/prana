/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_VM_UNARY_INTRINSICS_HPP)
#define BSP_VM_UNARY_INTRINSICS_HPP

#include <boost/spirit/home/prana/support/utree_rest.hpp>
#include <boost/spirit/home/prana/vm/core/unary_function.hpp>

namespace boost {
namespace spirit {
namespace prana {

#define BSP_UNARY_INTRINSIC(name, expression)                     \
  struct name##_function: unary_function<name##_function> {       \
    name##_function (function const& a):                          \
      base_type(a) { }                                            \
                                                                  \
    typedef utree result_type;                                    \
                                                                  \
    utree eval (utree const& element) const {                     \
      return expression;                                          \
    }                                                             \
  };                                                              \
                                                                  \
  struct name##_composite: unary_composite<name##_function> { };  \
                                                                  \
  name##_composite const name = name##_composite();               \
  /***/

BSP_UNARY_INTRINSIC(display, (std::cout << element, utree()))
BSP_UNARY_INTRINSIC(front,   element.front())
BSP_UNARY_INTRINSIC(back,    element.back())
BSP_UNARY_INTRINSIC(rest,    spirit::rest(element))
BSP_UNARY_INTRINSIC(negate,  !element)

#undef BSP_UNARY_INTRINSIC

} // prana
} // spirit
} // boost

#endif // BSP_VM_UNARY_INTRINSICS_HPP

