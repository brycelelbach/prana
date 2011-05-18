/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PHXPR_B370298D_35E9_4BF0_8256_4116A44BA04F)
#define PHXPR_B370298D_35E9_4BF0_8256_4116A44BA04F

#include <phxpr/primitives/procedure.hpp>

namespace phxpr {

struct argument_function: actor<argument_function> {
  typedef scope::size_type size_type;

  size_type arity;
  size_type index;
  size_type level;

  argument_function (size_type arity_, size_type index_, size_type level_):
    arity(arity_), index(index_), level(level_) { }

  utree eval (scope const& env) const {
    scope const* eptr = &env;

    // We want to find our parent level, so that we can access our scope through
    // it's array of children. Otherwise, we might access the wrong scope if
    // we've been moved around (remember, procedures are first class citizens!).
    while ((level + 1) != eptr->depth) {
      // TODO: replace with exception
      BOOST_ASSERT(eptr->parent)
      eptr = eptr->parent;
    }

    // Now, eptr is pointing to our parent scope. To get our scope, we need to
    // 

    utree const& arg = (*eptr)[n];

    if (arg.which() != utree_type::function_type)
      return utree(boost::ref(arg));
    else
      return arg.eval(*eptr);
  }
};

struct argument_composite {
  typedef argument_function::size_type size_type;

  typedef procedure result_type;

  result_type operator() (size_type arity, size_type index,
                          size_type level) const
  { return function(argument_function(arity, index, level)); }
};

argument_composite const argument = { };

} // phxpr

#endif // PHXPR_B370298D_35E9_4BF0_8256_4116A44BA04F

