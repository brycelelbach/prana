/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_VM_CORE_ARGUMENTS_HPP)
#define BSP_VM_CORE_ARGUMENTS_HPP

#include <boost/config.hpp>
#include <boost/ref.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repeat.hpp>

#include <boost/spirit/home/prana/vm/core/function.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<bool scoped = true>
struct argument_function: actor<argument_function<scoped> > {
  std::size_t n;
  std::size_t level;

  argument_function (std::size_t n, std::size_t level = 0):
    n(n), level(level) { }

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
};

template<> 
struct argument_function<false>: actor<argument_function<false> > {
  std::size_t n;

  argument_function (std::size_t n, std::size_t level = 0): n(n) { }

  utree eval (scope const& env) const {
    scope const* eptr = &env;
    utree const& arg = (*eptr)[n];

    if (arg.which() != utree_type::function_type)
      return utree(boost::ref(arg));
    else
      return arg.eval(*eptr);
  }
};

template<bool scoped = true>
struct argument {
  typedef function result_type;

  function operator() (std::size_t n, std::size_t level = 0) const {
    return function(argument_function<scoped>(n, level));
  }
};

// scoped arg
argument<true> const arg = { };

// unscoped arg
argument<false> const unscoped_arg = { };

#if !defined(BSP_ARGUMENT_LIMIT)
  #define BSP_ARGUMENT_LIMIT 8
#endif

#define BSP_UNSCOPED_ARG(z, n, data)            \
  function const BOOST_PP_CAT(_, n) = data(n);  \
  /***/

BOOST_PP_REPEAT(BSP_ARGUMENT_LIMIT,
                BSP_UNSCOPED_ARG,
                unscoped_arg)

#undef BSP_UNSCOPED_ARG

} // prana
} // spirit
} // boost

#endif // BSP_VM_CORE_ARGUMENTS_HPP

