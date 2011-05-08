/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_CORE_FUNCTION_HPP)
#define BSP_PHXPR_CORE_FUNCTION_HPP

#include <boost/spirit/home/prana/config.hpp>

#include <boost/spirit/home/prana/phxpr/core/actor.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct function: actor<function> {
  utree f;
  bool fixed;

  function (void): f() { std::cout << "ctor: " << this << std::endl; }

  function (utree const& f, bool fixed = true): f(f), fixed(fixed) { }

  function (function const& other): f(other.f), fixed(other.fixed) { }

  template<class F>
  function (F const& f, bool fixed = true):
    f(stored_function<F>(f)), fixed(fixed) { }

  ~function (void) { std::cout << "dtor: " << this << std::endl; }

  bool empty (void) const {
    return f.which() != utree_type::function_type;
  }

  bool fixed_arity (void) const {
    return fixed;
  }

  utree eval (scope const& env) const {
    return f.eval(env);
  }
};

// TODO: move to traits.hpp after new-api merge
template<>
struct extract_utree_from_type<function> {
  typedef utree type;

  static type call (function const& f) {
    return f.f;
  }
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_CORE_FUNCTION_HPP

