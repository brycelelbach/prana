/*==============================================================================
    Copyright (c) 2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PHXPR_77C41EF9_4258_471F_89E7_75087273BE79)
#define PHXPR_77C41EF9_4258_471F_89E7_75087273BE79

#include <phxpr/config.hpp>

#include <phxpr/exception.hpp>
#include <phxpr/signature.hpp>
#include <phxpr/utree/predicates.hpp>
#include <phxpr/primitives/actor.hpp>

namespace phxpr {

struct begin: actor<begin> {
  const signature sig;

  begin (evaluation_strategy::info eval = evaluation_strategy::call_by_value):
    sig(1, arity_type::variable, eval, function_type::derived) { }

  utree eval (scope const& args) const {
    using boost::fusion::at_c;

    // {{{ arity checking 
    if (at_c<0>(sig) != args.size())
      BOOST_THROW_EXCEPTION
        (invalid_arity(at_c<0>(sig), args.size(), arity_type::variable));
    // }}}

    if (!prana::is_utree_container(args[0]))
      BOOST_THROW_EXCEPTION(expected_variable_argument(at_c<0>(sig), args));

    return args[0].back();
  }
};

} // phxpr

#endif // PHXPR_77C41EF9_4258_471F_89E7_75087273BE79

