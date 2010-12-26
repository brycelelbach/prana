/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_CORE_FUNCTION_HPP)
#define BOOST_SPIRIT_PRANA_VM_CORE_FUNCTION_HPP

#include <boost/spirit/home/prana/vm/core/actor.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct function: actor<function> {
  utree f;

  function (void): f() { }

  function (utree const& f): f(f) { }

  template<class F>
  function (F const& f): f(stored_function<F>(f)) { }

  bool empty (void) const {
    return f.which() != utree_type::function_type;
  }

  utree eval (scope const& env) const {
    return f.eval(env);
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_CORE_FUNCTION_HPP

