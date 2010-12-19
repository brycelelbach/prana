/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_CORE_NARY_VARIABLE_ARGUMENTS_HPP)
#define BOOST_SPIRIT_PRANA_VM_CORE_NARY_VARIABLE_ARGUMENTS_HPP

#include <boost/config.hpp>
#include <boost/ref.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repeat.hpp>

#include <boost/spirit/home/prana/vm/core/function.hpp>

namespace boost {
namespace spirit {
namespace prana {

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
};

template<> 
struct vararg_function<false>: actor<vararg_function<false> > {
  std::size_t n;
  vararg_function (std::size_t n, std::size_t level = 0): n(n) { }

  utree eval (scope const& env) const {
    scope const* eptr = &env;
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
};

template<bool scoped = true>
struct vararg {
  typedef function result_type;

  function operator() (std::size_t n, std::size_t level = 0) const {
    return function(vararg_function<scoped>(n, level));
  }
};

// scoped varg
vararg<true> const varg = { };

// unscoped varg
vararg<false> const unscoped_varg = { };

#if !defined(BOOST_SPIRIT_PRANA_ARGUMENT_LIMIT)
  #define BOOST_SPIRIT_PRANA_ARGUMENT_LIMIT 8
#endif

#define BSP_UNSCOPED_VARG(z, n, data)                             \
  function const BOOST_PP_CAT(BOOST_PP_CAT(_, n), _) = data(n);   \
  /***/

BOOST_PP_REPEAT(BOOST_SPIRIT_PRANA_ARGUMENT_LIMIT,
                BSP_UNSCOPED_VARG,
                unscoped_varg)

#undef BSP_UNSCOPED_VARG

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_CORE_NARY_VARIABLE_ARGUMENTS_HPP

