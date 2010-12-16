/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_CORE_ACTOR_HPP)
#define BOOST_SPIRIT_PRANA_VM_CORE_ACTOR_HPP

#include <list>

#include <boost/array.hpp>

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct function;

typedef std::list<function> actor_list;

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
    return derived().eval(scope());
  }

  template<class A0>
  utree operator() (A0 const& _0) const {
    boost::array<utree, 1> elements;
    elements[0] = _0;
    return derived().eval(get_range(elements));
  }

  template<class A0, class A1>
  utree operator() (A0 const& _0, A1 const& _1) const {
    boost::array<utree, 2> elements;
    elements[0] = _0;
    elements[1] = _1;
    return derived().eval(get_range(elements));
  }

  // more operators
  #include <boost/spirit/home/prana/vm/function_call.hpp>
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_CORE_ACTOR_HPP

