/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PHXPR_81E48299_09EB_4B15_80D9_2052474D1E4E)
#define PHXPR_81E48299_09EB_4B15_80D9_2052474D1E4E

#include <phxpr/config.hpp>

#include <prana/include/utree.hpp>

namespace phxpr {

template <typename Derived>
struct actor {
  typedef actor<Derived> base_type;
  typedef utree result_type;

  Derived const& derived (void) const
  { return *static_cast<Derived const*>(this); }

  utree operator() (scope const& env) const
  { return derived().eval(env); }

  utree operator() (void) const
  { return derived().eval(scope(0, 0)); }
};

} // phxpr

#endif // PHXPR_81E48299_09EB_4B15_80D9_2052474D1E4E

