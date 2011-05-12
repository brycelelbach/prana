/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_BEGIN_INTRINSICS_HPP)
#define BSP_PHXPR_BEGIN_INTRINSICS_HPP

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

#include <boost/spirit/home/prana/phxpr/core/composite.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct begin_function: actor<begin_function> {
  boost::shared_ptr<actor_list> elements;

  begin_function (boost::shared_ptr<actor_list> const& elements_):
    elements(elements_)
  {
    BOOST_ASSERT(elements);
    BOOST_FOREACH(function const& element, *elements) {
      BOOST_ASSERT(!element.empty());
    }
  }

  utree eval (scope const& env) const {
    actor_list::const_iterator end = elements->end(); --end;
    iterator_range<actor_list::const_iterator>
      head_elements(elements->cbegin(), end);
    BOOST_FOREACH(function const& element, head_elements) {
      element(env);
    }
    return (*end)(env);
  }
};

struct begin_composite: composite<begin_composite> {
  function compose (boost::shared_ptr<actor_list> const& elements) const {
    // REVIEW: should minimum arity be 1?
    return function(begin_function(elements), 0, false);
  }
};

begin_composite const begin = begin_composite();

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_BEGIN_INTRINSICS_HPP

