/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_IF_INTRINSICS_HPP)
#define BOOST_SPIRIT_PRANA_VM_IF_INTRINSICS_HPP

#include <boost/assert.hpp>

#include <boost/spirit/home/prana/vm/core/composite.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct if_function: actor<if_function> {
  function cond;
  function then;
  function else_;

  if_function (function const& cond,
               function const& then,
               function const& else_): cond(cond), then(then), else_(else_) {
    BOOST_ASSERT(!cond.empty());
    BOOST_ASSERT(!then.empty());
    BOOST_ASSERT(!else_.empty());
  }

  utree eval (scope const& env) const {
    return cond(env).get<bool>() ? then(env) : else_(env);
  }
};

struct if_composite: composite<if_composite> {
  function compose (actor_list const& elements) const {
    actor_list::const_iterator i = elements.begin();
    function if_ = *i++;
    function then = *i++;
    function else_ = *i;
    return function(if_function(if_, then, else_));
  }
};

if_composite const if_ = if_composite();

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_IF_INTRINSICS_HPP

