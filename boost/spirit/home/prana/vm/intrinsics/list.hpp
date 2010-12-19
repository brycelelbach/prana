/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_LIST_INTRINSICS_HPP)
#define BOOST_SPIRIT_PRANA_VM_LIST_INTRINSICS_HPP

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

#include <boost/spirit/home/prana/vm/core/composite.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct list_function: actor<list_function> {
  actor_list elements;

  list_function (actor_list const& elements): elements(elements) {
    BOOST_FOREACH(function const & element, elements) {
      BOOST_ASSERT(!element.empty());
    }
  }

  utree eval (scope const& env) const {
    utree result;
    BOOST_FOREACH(function const & element, elements) {
      result.push_back(element(env));
    }
    return result;
  }
};

struct list_composite: composite<list_composite> {
  function compose (actor_list const& elements) const {
    return function(list_function(elements));
  }
};

list_composite const list = list_composite();

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_LIST_INTRINSICS_HPP

