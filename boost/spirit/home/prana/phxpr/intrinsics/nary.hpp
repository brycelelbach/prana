/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_NARY_INTRINSICS_HPP)
#define BSP_PHXPR_NARY_INTRINSICS_HPP

#include <boost/spirit/home/prana/phxpr/core/nary_function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

// TODO: enable short circut evaluation here.
#define BSP_NARY_INTRINSIC(name, expression)                          \
  struct name##_function: nary_function<name##_function> {            \
    name##_function (boost::shared_ptr<actor_list> const& elements):  \
      base_type(elements) { }                                         \
                                                                      \
    typedef bool result_type;                                         \
                                                                      \
    bool eval (utree& result, utree const& element) const {           \
      expression;                                                     \
      return true;                                                    \
    }                                                                 \
  };                                                                  \
                                                                      \
  struct name##_composite: nary_composite<name##_function> { };       \
                                                                      \
  name##_composite const name = name##_composite();                   \
  /***/

BSP_NARY_INTRINSIC(add,   result = result + element)
BSP_NARY_INTRINSIC(min,   result = result - element)
BSP_NARY_INTRINSIC(mul,   result = result * element)
BSP_NARY_INTRINSIC(div,   result = result / element)
BSP_NARY_INTRINSIC(eq,    result = result == element)
BSP_NARY_INTRINSIC(lt,    result = result < element)
BSP_NARY_INTRINSIC(lte,   result = result <= element)
BSP_NARY_INTRINSIC(and_,  result = result && element)
BSP_NARY_INTRINSIC(or_,   result = result || element)

#undef BSP_NARY_INTRINSIC

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_NARY_INTRINSICS_HPP

