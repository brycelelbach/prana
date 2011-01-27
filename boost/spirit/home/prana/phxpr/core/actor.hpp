/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_31AEA304_5138_4C2B_9166_3BE660EDF2D2)
#define BSP_31AEA304_5138_4C2B_9166_3BE660EDF2D2

#include <boost/spirit/home/prana/config.hpp>

#include <boost/array.hpp>
#include <boost/scoped_array.hpp>
#include <boost/foreach.hpp>

#include <boost/spirit/include/support_utree.hpp>

#include <boost/spirit/home/prana/adt/dynamic_array.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct function;

typedef dynamic_array<function> actor_list;

// TODO: move to traits.hpp after new-api merge
template<class T, class Enable = void>
struct extract_utree_from_type {
  typedef utree type;

  static type call (T const& val) {
    return utree(val);
  }
};

inline utree& as_utree (utree& ut) {
  return ut;
}

inline utree const& as_utree (utree const& ut) {
  return ut;
}

template<class T>
inline utree as_utree (T const& val) {
  return extract_utree_from_type<T>::call(val);
}

template<std::size_t n>
inline scope get_range (boost::array<utree, n>& array) {
  return scope(array.begin(), array.end());
}

template<class Derived>
struct actor {
  typedef actor<Derived> base_type;
  typedef utree result_type;

  Derived const& derived (void) const {
    return *static_cast<Derived const*>(this);
  }

  utree operator() (scope const& env) const {
    return derived().eval(env);
  }

  utree operator() (void) const {
    boost::array<utree, 1> elements;
    elements[0] = derived().clone();
    return derived().eval(get_range(elements));
  }

  template<class A0>
  utree operator() (A0 const& _0) const {
    boost::array<utree, 2> elements;
    elements[0] = derived().clone();
    elements[1] = as_utree(_0);
    return derived().eval(get_range(elements));
  }

  template<class A0, class A1>
  utree operator() (A0 const& _0, A1 const& _1) const {
    boost::array<utree, 3> elements;
    elements[0] = derived().clone();
    elements[1] = as_utree(_0);
    elements[2] = as_utree(_1);
    return derived().eval(get_range(elements));
  }

  #include <boost/spirit/home/prana/phxpr/vertical/function_call.hpp>
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_31AEA304_5138_4C2B_9166_3BE660EDF2D2

