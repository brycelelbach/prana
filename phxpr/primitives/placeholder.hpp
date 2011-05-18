/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PHXPR_65116B3F_9B1E_44B4_9602_7DBCA555427B)
#define PHXPR_65116B3F_9B1E_44B4_9602_7DBCA555427B

#include <phxpr/config.hpp>

#include <boost/ref.hpp>

#include <prana/utree/predicates.hpp>

#include <phxpr/signature.hpp>
#include <phxpr/primitives/actor.hpp>
#include <phxpr/exception.hpp>

namespace phxpr {

struct placeholder: actor<placeholder> {
  displacement n;

  placeholder (displacement n_): n(n_) { } 

  utree eval (scope const& env) const {
    if (env.size() <= n)
      BOOST_THROW_EXCEPTION(invalid_arity(n, env.size(), arity_type::fixed));

    return utree(boost::ref(env[n]));
  }
};

} // phxpr

#endif // PHXPR_B370298D_35E9_4BF0_8256_4116A44BA04F

