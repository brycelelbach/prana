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

#include <prana/utree/io.hpp>
#include <prana/utree/predicates.hpp>

#include <phxpr/signature.hpp>
#include <phxpr/primitives/actor.hpp>
#include <phxpr/exception.hpp>

namespace phxpr {

struct placeholder: actor<placeholder> {
  displacement n;
  displacement frame;

  placeholder (displacement n_, displacement frame_): n(n_), frame(frame_) { } 

  utree eval (scope const& env) const {
    boost::shared_ptr<scope> eptr = env.get();

    BOOST_ASSERT(eptr);

    while (frame != eptr->level()) {
      BOOST_ASSERT(eptr);
      eptr = eptr->outer();
    } 

    if (eptr->size() <= n) {
      std::cout << "eptr size: " << eptr->size() << std::endl
                << "n: " << n << std::endl
                << env << std::endl;
      BOOST_THROW_EXCEPTION
        (invalid_placeholder(n, frame, eptr->size(), arity_type::fixed));
    }

    return utree((*eptr)[n]);
  }
};

} // phxpr

#endif // PHXPR_B370298D_35E9_4BF0_8256_4116A44BA04F

