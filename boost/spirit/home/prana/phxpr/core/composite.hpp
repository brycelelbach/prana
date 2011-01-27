/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_CORE_COMPOSITE_HPP)
#define BSP_PHXPR_CORE_COMPOSITE_HPP

#include <boost/spirit/home/prana/config.hpp>

#include <boost/shared_ptr.hpp>

#include <boost/spirit/home/prana/phxpr/core/quote.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

// TODO: convert to CP and move to traits.hpp after new-api merge
template<class T>
inline function as_function (T const& val) {
  return quote(utree(val));
}

inline function const& as_function (function const& f) {
  return f;
}

template<class Derived, class Result = function>
struct composite {
  typedef composite<Derived> base_type;
  typedef Result result_type;

  Derived const& derived (void) const {
    return *static_cast<Derived const*>(this);
  }
  
  result_type operator() (boost::shared_ptr<actor_list> const& elements) const {
    return derived().compose(elements);
  }

  result_type operator() (actor_list const& actors) const {
    boost::shared_ptr<actor_list> elements(new actor_list(actors));
    return derived().compose(elements);
  }

  template<class A0>
  result_type operator() (A0 const& _0) const {
    boost::shared_ptr<actor_list> elements(new actor_list(1));
    elements->push_back(as_function(_0));
    return derived().compose(elements);
  }

  template<class A0, class A1>
  result_type operator() (A0 const& _0, A1 const& _1) const {
    boost::shared_ptr<actor_list> elements(new actor_list(2));
    elements->push_back(as_function(_0));
    elements->push_back(as_function(_1));
    return derived().compose(elements);
  }

  #include <boost/spirit/home/prana/phxpr/vertical/composite_call.hpp>
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_CORE_COMPOSITE_HPP

