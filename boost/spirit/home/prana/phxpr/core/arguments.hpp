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
  typedef scope::size_type size_type;

  size_type n;
  size_type level;

  argument_function (size_type n_, size_type level_ = 0):
    n(n_), level(level_) { }

  utree eval (scope const& env) const {
    scope const* eptr = &env;

    while (level != eptr->level())
      eptr = eptr->outer();

    utree const& arg = (*eptr)[n];

    if (arg.which() != utree_type::function_type) {
//      std::cout << "evaluating non-function" << std::endl
//                << "scoped arg (n=" << n << " level=" << level << ") "
//                << arg.which() << std::endl;
      return utree(boost::ref(arg));
    }
    else {
//      std::cout << "evaluating function" << std::endl
//                << "scoped arg (n=" << n << " level=" << level << ") "
//                << arg.which() << std::endl;
      return arg.eval(*eptr);
    }
  }
  
  utree get (scope const& env) const {
    scope const* eptr = &env;

    while (level != eptr->level())
      eptr = eptr->outer();

    utree const& arg = (*eptr)[n];
    return utree(boost::ref(arg));
  }
};

template<> 
struct argument_function<false>: actor<argument_function<false> > {
  typedef scope::size_type size_type;

  size_type n;

  argument_function (size_type n_, size_type level_ = 0):
    n(n_) { }

  utree eval (scope const& env) const {
    utree const& arg = env[n];

    if (arg.which() != utree_type::function_type)
      return utree(boost::ref(arg));
    else 
      return arg.eval(env);
  }
  
  utree get (scope const& env) const {
    utree const& arg = env[n];
    return utree(boost::ref(arg));
  }
};

template<bool scoped = true>
struct argument {
  typedef scope::size_type size_type;

  typedef function result_type;

  function operator() (size_type n, size_type level = 0) const {
    return function(argument_function<scoped>(n, level), n + 1, true, level);
  }
};

// scoped arg
argument<true> const arg = { };

// unscoped arg
argument<false> const unscoped_arg = { };

function const _1 = unscoped_arg(0);
function const _2 = unscoped_arg(1);

#define BSP_MAKE_PLACEHOLDER(n)                                     \
  function const BOOST_PP_CAT(_, n) = unscoped_arg(BOOST_PP_DEC(n)) \
  /***/

#include <boost/spirit/home/prana/phxpr/vertical/placeholders.hpp>

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_CORE_ARGUMENTS_HPP

