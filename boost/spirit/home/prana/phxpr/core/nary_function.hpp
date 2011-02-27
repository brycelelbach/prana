/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_CORE_NARY_FUNCTION_HPP)
#define BSP_PHXPR_CORE_NARY_FUNCTION_HPP

#include <boost/spirit/home/prana/config.hpp>

#include <boost/spirit/home/prana/phxpr/core/composite.hpp>

#include <boost/foreach.hpp>
#include <boost/assert.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

template<class Derived>
struct nary_function: actor<nary_function<Derived> > {
  boost::shared_ptr<actor_list> elements;

  typedef nary_function<Derived> base_type;

  Derived const& derived (void) const {
    return *static_cast<Derived const*>(this);
  }

  nary_function (boost::shared_ptr<actor_list> const& elements,
                 scope::size_type minimum_arity = 0): elements(elements)
  {
    BOOST_ASSERT(elements);
    BOOST_ASSERT(elements->size() >= minimum_arity);
    BOOST_FOREACH(function const& element, *elements) {
      BOOST_ASSERT(!element.empty());
    }
  }

  utree eval (scope const& env) const {
    actor_list::const_iterator i = elements->begin();
    utree result = (*i++)(env);
    boost::iterator_range<actor_list::const_iterator>
      rest(i++, elements->cend());

    BOOST_FOREACH(function const& element, rest) {
      if (!derived().eval(result, element(env)))
        break; // allow short-circuit evaluation
    }

    return result;
  }
  
  utree clone (void) const {
    return utree(stored_function<nary_function>(
      nary_function(elements)));
  }
};

template<class Function>
struct nary_composite: composite<nary_composite<Function> > {
  function compose (boost::shared_ptr<actor_list> const& elements) const {
    return function(Function(elements));
  }
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_CORE_NARY_FUNCTION_HPP
