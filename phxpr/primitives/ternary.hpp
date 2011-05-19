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

namespace phxpr {

template <typename Derived>
struct ternary: actor<ternary<Derived> > {
  const signature sig;

  typedef ternary<Derived> base_type;

  Derived const& derived (void) const
  { return *static_cast<Derived const*>(this); }

  ternary (evaluation_strategy::info eval = evaluation_strategy::call_by_value):
    sig(3, arity_type::fixed, eval, function_type::derived) { }

  utree eval (scope const& args) const {
    using boost::fusion::at_c;

    // {{{ arity checking (TODO: variable arity)
    if (at_c<0>(sig) != args.size())
      BOOST_THROW_EXCEPTION
        (invalid_arity(at_c<0>(sig), args.size(), arity_type::fixed));
    // }}}

    return derived().eval(args[0], args[1], args[2]);
  }
};

} // phxpr

#endif // PHXPR_373CA4E2_0950_402D_AF5D_7A49EE0AEA05

