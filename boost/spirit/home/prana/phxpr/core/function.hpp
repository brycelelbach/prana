/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_CORE_FUNCTION_HPP)
#define BSP_PHXPR_CORE_FUNCTION_HPP

#include <boost/ref.hpp>

#include <boost/spirit/home/prana/config.hpp>

#include <boost/spirit/home/prana/support/utree_predicates.hpp>
#include <boost/spirit/home/prana/phxpr/core/actor.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct function: actor<function> {
  typedef scope::size_type size_type;

  utree f;
  size_type arity;
  bool fixed;
  size_type level;

  function (void): f() { }

  function (utree const& f_, size_type arity_ = 0, bool fixed_ = true,
            size_type level_ = size_type(~0)):
    f(f_), arity(arity_), fixed(fixed_), level(level_) { }

  function (function const& other):
    f(other.f), arity(other.arity), fixed(other.fixed), level(other.level) { }

  template<class F>
  function (F const& f_, size_type arity_ = 0, bool fixed_ = true,
            size_type level_ = size_type(~0)):
    f(stored_function<F>(f_)), arity(arity_), fixed(fixed_), level(level_) { }

  bool empty (void) const {
    return f.which() == utree_type::invalid_type;
  }

  utree eval (scope const& env) const {
    // TODO: arity/scope checking
    if (recursive_which(f) == utree_type::function_type)
      return f.eval(env);
    else
      return utree(boost::ref(f));
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

