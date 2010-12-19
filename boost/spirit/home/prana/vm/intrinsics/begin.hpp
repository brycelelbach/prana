/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_BEGIN_INTRINSICS_HPP)
#define BOOST_SPIRIT_PRANA_VM_BEGIN_INTRINSICS_HPP

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

#include <boost/spirit/home/prana/vm/core/composite.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct begin_function: actor<begin_function> {
  actor_list elements;

  begin_function (actor_list const& elements): elements(elements) {
    BOOST_FOREACH(function const & element, elements) {
      BOOST_ASSERT(!element.empty());
    }
  }

  utree eval (scope const& env) const {
    BOOST_ASSERT(!elements.empty());
    actor_list::const_iterator end = elements.end();
    --end;
    iterator_range<actor_list::const_iterator>
    head_elements(elements.begin(), end);
    BOOST_FOREACH(function const & element, head_elements) {
      element(env);
    }
    return (*end)(env);
  }
};

struct begin_composite: composite<begin_composite> {
  function compose (actor_list const& elements) const {
    return function(begin_function(elements));
  }
};

begin_composite const begin = begin_composite();

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_BEGIN_INTRINSICS_HPP

