/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_CORE_NARY_FUNCTION_HPP)
#define BOOST_SPIRIT_PRANA_VM_CORE_NARY_FUNCTION_HPP

#include <boost/spirit/home/prana/vm/core/composite.hpp>

#include <boost/foreach.hpp>
#include <boost/assert.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Derived>
struct nary_function: actor<nary_function<Derived> > {
  actor_list elements;

  typedef nary_function<Derived> base_type;

  Derived const& derived (void) const {
    return *static_cast<Derived const*>(this);
  }

  nary_function (actor_list const& elements): elements(elements) {
    BOOST_FOREACH(function const & element, elements) {
      BOOST_ASSERT(!element.empty());
    }
  }

  utree eval (scope const& env) const {
    BOOST_ASSERT(!elements.empty());

    actor_list::const_iterator i = elements.begin();
    utree result = (*i++)(env);
    boost::iterator_range<actor_list::const_iterator> rest(i++, elements.end());

    BOOST_FOREACH(function const & element, rest) {
      if (!derived().eval(result, element(env)))
        break; // allow short-circuit evaluation
    }

    return result;
  }
};

template<class Function>
struct nary_composite: composite<nary_composite<Function> > {
  function compose (actor_list const& elements) const {
    return function(Function(elements));
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_CORE_NARY_FUNCTION_HPP

