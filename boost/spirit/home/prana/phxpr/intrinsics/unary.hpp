/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_UNARY_INTRINSICS_HPP)
#define BSP_PHXPR_UNARY_INTRINSICS_HPP

#include <boost/spirit/home/prana/support/utree_rest.hpp>
#include <boost/spirit/home/prana/phxpr/core/unary_function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

#define BSP_UNARY_INTRINSIC(name, expression)                         \
  struct name##_function: unary_function<name##_function> {           \
    name##_function (boost::shared_ptr<actor_list> const& elements):  \
      base_type(elements) { }                                         \
                                                                      \
    typedef utree result_type;                                        \
                                                                      \
    utree eval (utree const& subject) const {                         \
      return expression;                                              \
    }                                                                 \
  };                                                                  \
                                                                      \
  struct name##_composite: unary_composite<name##_function> { };      \
                                                                      \
  name##_composite const name = name##_composite();                   \
  /***/

BSP_UNARY_INTRINSIC(empty,   subject.empty())
BSP_UNARY_INTRINSIC(front,   subject.front())
BSP_UNARY_INTRINSIC(back,    subject.back())
BSP_UNARY_INTRINSIC(rest,    spirit::rest(subject))
BSP_UNARY_INTRINSIC(not_,    !subject)

// temporary intrinsic, superseded by ports
BSP_UNARY_INTRINSIC(display, (std::cout << subject, utree(spirit::nil)))

#undef BSP_UNARY_INTRINSIC

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_UNARY_INTRINSICS_HPP

