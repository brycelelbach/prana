/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_IF_INTRINSICS_HPP)
#define BSP_PHXPR_IF_INTRINSICS_HPP

#include <boost/assert.hpp>

#include <boost/spirit/home/prana/phxpr/core/composite.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct if_function: actor<if_function> {
  boost::shared_ptr<actor_list> elements;

  if_function (boost::shared_ptr<actor_list> const& elements_):
    elements(elements_)            
  {
    BOOST_ASSERT(elements);
    BOOST_ASSERT(elements->size() == 3);
    BOOST_FOREACH(function const& element, *elements) {
      BOOST_ASSERT(!element.empty());
    }
  }

  utree eval (scope const& env) const {
    // TODO: typecheck (*elements)[0](env)
    if ((*elements)[0](env).get<bool>())
      return (*elements)[1](env);
    else
      return (*elements)[2](env);
  }
};

struct if_composite: composite<if_composite> {
  function compose (boost::shared_ptr<actor_list> const& elements) const {
    return function(if_function(elements));
  }
};

if_composite const if_ = if_composite();

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_IF_INTRINSICS_HPP

