/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_CORE_LAMBDA_HPP)
#define BOOST_SPIRIT_PRANA_VM_CORE_LAMBDA_HPP

#include <boost/spirit/home/prana/vm/core/composite.hpp>

#include <boost/scoped_array.hpp>
#include <boost/foreach.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct lambda_function: actor<lambda_function> {
  int level;
  actor_list elements;
  reference_wrapper<function const> f; // We must hold f by reference because
                                       // functions can be recursive.

  typedef utree result_type;

  lambda_function (function const& f,
                   actor_list const& elements,
                   int level = 0):
    level(level), elements(elements), f(f) { }

  utree eval (scope const& env) const {
    scope const* outer = &env; /*< Get the parent scope >*/

    while (level != outer->level())
      outer = outer->outer();

    if (!elements.empty()) {
      boost::scoped_array<utree> fargs(new utree[elements.size()]);

      std::size_t i = 0;
      BOOST_FOREACH(function const& element, elements) {
        fargs[i++] = element(env);
      }

      utree* fi = fargs.get();

      return f.get()(scope(fi, fi + elements.size(), outer));
    }
    
    else
      return f.get()(scope(0, 0, outer));
  }
};

struct lambda: composite<lambda> {
  function f;

  lambda (void): f() { }
  lambda (function const& f): f(f) { }

  function compose (actor_list const& elements) const {
    return function(lambda_function(f, elements));
  }

  lambda& operator= (lambda const& other) {
    f = other.f;
    return *this;
  }

  lambda& operator= (function const& f_) {
    f = f_;
    return *this;
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_CORE_LAMBDA_HPP

