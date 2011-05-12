/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_CORE_UNARY_FUNCTION_HPP)
#define BSP_PHXPR_CORE_UNARY_FUNCTION_HPP

#include <boost/spirit/home/prana/config.hpp>

#include <boost/spirit/home/prana/phxpr/core/composite.hpp>

#include <boost/assert.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

template<class Derived>
struct unary_function: actor<unary_function<Derived> > {
  boost::shared_ptr<actor_list> elements;

  typedef unary_function<Derived> base_type;
  
  Derived const& derived (void) const {
    return *static_cast<Derived const*>(this);
  }

  unary_function (boost::shared_ptr<actor_list> const& elements_):
    elements(elements_)
  {
    BOOST_ASSERT(elements);
    BOOST_ASSERT(elements->size() == 1);
    BOOST_FOREACH(function const& element, *elements) {
      BOOST_ASSERT(!element.empty());
    }
  }

  utree eval (scope const& env) const {
    return derived().eval((*elements)[0](env));
  }
};

template<class Function>
struct unary_composite: composite<unary_composite<Function> > {
  function compose (boost::shared_ptr<actor_list> const& elements) const {
    return function(Function(elements), 1, true);
  }
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_CORE_UNARY_FUNCTION_HPP

