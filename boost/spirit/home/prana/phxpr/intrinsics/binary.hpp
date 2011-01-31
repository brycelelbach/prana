/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_6FEECC05_8584_4C14_BBAF_A09941517FE4)
#define BSP_6FEECC05_8584_4C14_BBAF_A09941517FE4

#include <boost/spirit/home/prana/support/utree_index.hpp>
#include <boost/spirit/home/prana/phxpr/core/binary_function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

#define BSP_BINARY_INTRINSIC(name, expression)                        \
  struct name##_function: binary_function<name##_function> {          \
    name##_function (boost::shared_ptr<actor_list> const& elements):  \
      base_type(elements) { }                                         \
                                                                      \
    typedef utree result_type;                                        \
                                                                      \
    result_type eval (utree const& left, utree const& right) const {  \
      return expression;                                              \
    }                                                                 \
  };                                                                  \
                                                                      \
  struct name##_composite: binary_composite<name##_function> { };     \
                                                                      \
  name##_composite const name = name##_composite();                   \
  /***/

BSP_BINARY_INTRINSIC(at,    spirit::index(left, right))
BSP_BINARY_INTRINSIC(eq,    left == right)
BSP_BINARY_INTRINSIC(lt,    left < right)
BSP_BINARY_INTRINSIC(lte,   left <= right)
BSP_BINARY_INTRINSIC(and_,  left && right)
BSP_BINARY_INTRINSIC(or_,   left || right)

#undef BSP_BINARY_INTRINSIC

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_6FEECC05_8584_4C14_BBAF_A09941517FE4

