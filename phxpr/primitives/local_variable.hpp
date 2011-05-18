/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PHXPR_3AD160AB_32B1_4567_82C0_B1A3C8EADA0F)
#define PHXPR_3AD160AB_32B1_4567_82C0_B1A3C8EADA0F

#include <phxpr/config.hpp>

#include <boost/ref.hpp>

#include <prana/utree/predicates.hpp>

#include <phxpr/signature.hpp>
#include <phxpr/primitives/actor.hpp>
#include <phxpr/exception.hpp>

namespace phxpr {

struct local_variable: actor<local_variable> {
  displacement n;

  local_variable (displacement n_): n(n_) { } 

  utree eval (scope const& env) const {
    if (env.size() <= n)
      BOOST_THROW_EXCEPTION(invalid_local_variable(n, env.size()));

    return utree(boost::ref(env[n]));
  }
};

} // phxpr

#endif // PHXPR_3AD160AB_32B1_4567_82C0_B1A3C8EADA0F

