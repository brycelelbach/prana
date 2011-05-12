/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_CORE_NARY_VARIABLE_ARGUMENTS_HPP)
#define BSP_PHXPR_CORE_NARY_VARIABLE_ARGUMENTS_HPP

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
struct vararg_function: actor<vararg_function<scoped> > {
  typedef scope::size_type size_type;

  size_type n;
  size_type level;

  vararg_function (size_type n_, size_type level_ = 0): n(n_), level(level_) { }

  utree eval (scope const& env) const {
    scope const* eptr = &env;

    while (level != eptr->level())
      eptr = eptr->outer();

    utree result;

    for (size_type i = n; i < eptr->size(); ++i) {
      utree const& arg = (*eptr)[i];

      if (arg.which() != utree_type::function_type)
        result.push_back(utree(boost::ref(arg)));
      else
        result.push_back(arg.eval(*eptr));
    }

    return result;
  }
};

template<> 
struct vararg_function<false>: actor<vararg_function<false> > {
  typedef scope::size_type size_type;

  size_type n;

  vararg_function (size_type n_, size_type level_ = 0): n(n_) { }

  utree eval (scope const& env) const {
    utree result;
    
    for (size_type i = n; i < env.size(); ++i) {
      utree const& arg = env[i];

      if (arg.which() != utree_type::function_type)
        result.push_back(utree(boost::ref(arg)));
      else 
        result.push_back(arg.eval(env));
    }

    return result;
  }
};

template<bool scoped = true>
struct vararg {
  typedef scope::size_type size_type;

  typedef function result_type;

  function operator() (size_type n, size_type level = 0) const {
    return function(vararg_function<scoped>(n, level), n + 1, false, level);
  }
};

// scoped varg
vararg<true> const varg = { };

// unscoped varg
vararg<false> const unscoped_varg = { };

function const _1_ = unscoped_varg(0);
function const _2_ = unscoped_varg(1);

#define BSP_MAKE_PLACEHOLDER(n)                         \
  function const BOOST_PP_CAT(BOOST_PP_CAT(_, n), _) =  \
    unscoped_varg(BOOST_PP_DEC(n));                     \
  /***/

#include <boost/spirit/home/prana/phxpr/vertical/placeholders.hpp>

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_CORE_NARY_VARIABLE_ARGUMENTS_HPP

