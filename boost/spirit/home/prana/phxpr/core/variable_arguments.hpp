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
  std::size_t n;
  std::size_t level;

  vararg_function (std::size_t n, std::size_t level = 0): n(n), level(level) { }

  utree eval (scope const& env) const {
    scope const* eptr = &env;

    while (level != eptr->level())
      eptr = eptr->outer();
    
    utree result;

    for (std::size_t i = n; i < eptr->size(); ++i) {
      utree const& arg = (*eptr)[i];

      if (arg.which() != utree_type::function_type)
        result.push_back(utree(boost::ref(arg)));
      else
        result.push_back(arg.eval(*eptr));
    }

    return result;
  }
  
  utree clone (void) const {
    return utree(stored_function<vararg_function>(
      vararg_function(n, level)));
  }
};

template<> 
struct vararg_function<false>: actor<vararg_function<false> > {
  std::size_t n;

  vararg_function (std::size_t n, std::size_t level = 0): n(n) { }

  utree eval (scope const& env) const {
    utree result;
    
    for (std::size_t i = n; i < env.size(); ++i) {
      utree const& arg = env[i];

      if (arg.which() != utree_type::function_type)
        result.push_back(utree(boost::ref(arg)));
      else 
        result.push_back(arg.eval(env));
    }

    return result;
  }
  
  utree clone (void) const {
    return utree(stored_function<vararg_function>(
      vararg_function(n)));
  }
};

template<bool scoped = true>
struct vararg {
  typedef function result_type;

  function operator() (std::size_t n, std::size_t level = 0) const {
    return function(vararg_function<scoped>(n, level), false);
  }
};

// scoped varg
vararg<true> const varg = { };

// unscoped varg
vararg<false> const unscoped_varg = { };

function const _1_ = unscoped_varg(1);
function const _2_ = unscoped_varg(2);

#define BSP_MAKE_PLACEHOLDER(n)                         \
  function const BOOST_PP_CAT(BOOST_PP_CAT(_, n), _) =  \
    unscoped_varg(n);                                   \
  /***/

#include <boost/spirit/home/prana/phxpr/vertical/placeholders.hpp>

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_CORE_NARY_VARIABLE_ARGUMENTS_HPP

