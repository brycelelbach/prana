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
  typedef scope::size_type size_type;

  boost::shared_ptr<actor_list> elements;

  typedef nary_function<Derived> base_type;

  Derived const& derived (void) const {
    return *static_cast<Derived const*>(this);
  }

  nary_function (boost::shared_ptr<actor_list> const& elements,
                 size_type minimum_arity = 0): elements(elements)
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
};

template<class Function>
struct nary_composite: composite<nary_composite<Function> > {
  typedef scope::size_type size_type;

  size_type const minimum;

  nary_composite (size_type minimum_ = 0): minimum(minimum_) { }

  function compose (boost::shared_ptr<actor_list> const& elements) const {
    return function(Function(elements, minimum), minimum, false);
  }
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_CORE_NARY_FUNCTION_HPP

