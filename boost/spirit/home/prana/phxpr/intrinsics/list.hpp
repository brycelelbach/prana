/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_LIST_INTRINSICS_HPP)
#define BSP_PHXPR_LIST_INTRINSICS_HPP

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

#include <boost/spirit/home/prana/phxpr/core/composite.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct list_function: actor<list_function> {
  boost::shared_ptr<actor_list> elements;

  list_function (boost::shared_ptr<actor_list> const& elements_):
    elements(elements_)
  {
    BOOST_ASSERT(elements);
    BOOST_FOREACH(function const& element, *elements) {
      BOOST_ASSERT(!element.empty());
    }
  }

  utree eval (scope const& env) const {
    utree result;
    BOOST_FOREACH(function const& element, *elements) {
      result.push_back(element(env));
    }
    return result;
  }
};

struct list_composite: composite<list_composite> {
  function compose (boost::shared_ptr<actor_list> const& elements) const {
    return function(list_function(elements), 0, false);
  }
};

list_composite const list = list_composite();

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_LIST_INTRINSICS_HPP

