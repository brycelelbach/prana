/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_CORE_ARGUMENTS_HPP)
#define BSP_PHXPR_CORE_ARGUMENTS_HPP

#include <boost/spirit/home/prana/config.hpp>

#include <boost/ref.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/dec.hpp>

#include <boost/spirit/home/prana/phxpr/core/function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

template<bool scoped = true>
struct argument_function: actor<argument_function<scoped> > {
  scope::size_type n;
  scope::size_type level;

  argument_function (scope::size_type n_, scope::size_type level_ = 0):
    n(n_), level(level_) { }

  utree eval (scope const& env) const {
    scope const* eptr = &env;

    while (level != eptr->level())
      eptr = eptr->outer();

    utree const& arg = (*eptr)[n];

    if (arg.which() != utree_type::function_type)
      return utree(boost::ref(arg));
    else
      return arg.eval(*eptr);
  }
  
  utree clone (void) const {
    return utree(stored_function<argument_function>(
      argument_function(n, level)));
  }
};

template<> 
struct argument_function<false>: actor<argument_function<false> > {
  scope::size_type n;

  argument_function (scope::size_type n_, scope::size_type level_ = 0):
    n(n_) { }

  utree eval (scope const& env) const {
    utree const& arg = env[n];

    if (arg.which() != utree_type::function_type)
      return utree(boost::ref(arg));
    else 
      return arg.eval(env);
  }
  
  utree clone (void) const {
    return utree(stored_function<argument_function>(
      argument_function(n)));
  }
};

template<bool scoped = true>
struct argument {
  typedef function result_type;

  function operator() (scope::size_type n, scope::size_type level = 0) const {
    return function(argument_function<scoped>(n, level));
  }
};

// scoped arg
argument<true> const arg = { };

// unscoped arg
argument<false> const unscoped_arg = { };

function const _1 = unscoped_arg(1);
function const _2 = unscoped_arg(2);

#define BSP_MAKE_PLACEHOLDER(n)                       \
  function const BOOST_PP_CAT(_, n) = unscoped_arg(n) \
  /***/

#include <boost/spirit/home/prana/phxpr/vertical/placeholders.hpp>

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_CORE_ARGUMENTS_HPP

