/*==============================================================================
    Copyright (c) 2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PHXPR_373CA4E2_0950_402D_AF5D_7A49EE0AEA05)
#define PHXPR_373CA4E2_0950_402D_AF5D_7A49EE0AEA05

#include <phxpr/config.hpp>

#include <phxpr/exception.hpp>
#include <phxpr/signature.hpp>
#include <phxpr/primitives/actor.hpp>
#include <phxpr/environment.hpp>

namespace phxpr {

template <typename Derived>
struct ternary: actor<ternary<Derived> > {
  const signature sig;

  typedef ternary<Derived> base_type;

  Derived const& derived (void) const
  { return *static_cast<Derived const*>(this); }

  ternary (evaluation_strategy::info stra = evaluation_strategy::call_by_value):
    sig(3, arity_type::fixed, stra, function_type::derived, 0) { }

  utree eval (utree& ut) const {
    using boost::fusion::at_c;
    
    runtime_environment& env = *ut.get<runtime_environment*>();

#if 0
    // {{{ arity checking 
    if (at_c<0>(sig) != env.size())
      BOOST_THROW_EXCEPTION
        (invalid_arity(at_c<0>(sig), env.size(), arity_type::fixed));
    // }}}
#endif

    return derived().eval(env[0], env[1], env[2]);
  }

  function_base* copy (void) const
  { return derived().duplicate(); }
};

} // phxpr

#endif // PHXPR_373CA4E2_0950_402D_AF5D_7A49EE0AEA05

