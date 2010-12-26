/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_CORE_COMPOSITE_HPP)
#define BOOST_SPIRIT_PRANA_VM_CORE_COMPOSITE_HPP

#include <boost/spirit/home/prana/vm/core/value.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class T>
inline function as_function (T const& val) {
  return val(utree(val));
}

inline function const& as_function (function const& f) {
  return f;
}

template<class Derived>
struct composite {
  typedef composite<Derived> base_type;
  typedef function result_type;

  Derived const& derived (void) const {
    return *static_cast<Derived const*>(this);
  }

  function operator() (actor_list const& elements) const {
    return derived().compose(elements);
  }

  template<class A0>
  function operator() (A0 const& _0) const {
    actor_list elements;
    elements.push_back(as_function(_0));
    return derived().compose(elements);
  }

  template<class A0, class A1>
  function operator() (A0 const& _0, A1 const& _1) const {
    actor_list elements;
    elements.push_back(as_function(_0));
    elements.push_back(as_function(_1));
    return derived().compose(elements);
  }

  // More operators
  #include <boost/spirit/home/prana/vm/composite_call.hpp>
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_CORE_COMPOSITE_HPP

