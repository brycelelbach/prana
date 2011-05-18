/*==============================================================================
    Copyright (c) 2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PHXPR_5B266627_F3E6_4B43_A515_7484A3D2B41C)
#define PHXPR_5B266627_F3E6_4B43_A515_7484A3D2B41C

#include <phxpr/config.hpp>

#include <phxpr/exception.hpp>
#include <phxpr/signature.hpp>
#include <phxpr/primitives/actor.hpp>

namespace phxpr {

template <typename Derived>
struct binary: actor<binary<Derived> > {
  const signature sig;

  typedef binary<Derived> base_type;

  Derived const& derived (void) const
  { return *static_cast<Derived const*>(this); }

  binary (evaluation_strategy::info eval = evaluation_strategy::call_by_value):
    sig(2, arity_type::fixed, eval) { }

  utree eval (scope const& args) const {
    using boost::fusion::at_c;

    // {{{ arity checking (TODO: variable arity)
    if (at_c<0>(sig) != args.size())
      BOOST_THROW_EXCEPTION
        (invalid_arity(at_c<0>(sig), args.size(), arity_type::fixed));
    // }}}

    return derived().eval(args[0], args[1]);
  }
};

} // phxpr

#endif // PHXPR_5B266627_F3E6_4B43_A515_7484A3D2B41C

