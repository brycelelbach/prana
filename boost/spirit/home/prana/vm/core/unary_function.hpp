/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_CORE_UNARY_FUNCTION_HPP)
#define BOOST_SPIRIT_PRANA_VM_CORE_UNARY_FUNCTION_HPP

#include <boost/spirit/home/prana/vm/core/composite.hpp>

#include <boost/assert.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Derived>
struct unary_function: actor<unary_function<Derived> > {
  function x;

  typedef unary_function<Derived> base_type;
  
  Derived const& derived (void) const {
    return *static_cast<Derived const*>(this);
  }

  unary_function (function const& x): x(x) {
    BOOST_ASSERT(!x.empty());
  }

  utree eval (scope const& env) const {
    return derived().eval(x(env));
  }
};

template<class Function>
struct unary_composite: composite<unary_composite<Function> > {
  function compose (actor_list const& elements) const {
    return function(Function(elements.front()));
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_CORE_UNARY_FUNCTION_HPP

