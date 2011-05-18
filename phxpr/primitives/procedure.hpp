/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PHXPR_9E9F37AF_C618_4FD0_A51B_559364CF9827)
#define PHXPR_9E9F37AF_C618_4FD0_A51B_559364CF9827

#include <boost/ref.hpp>

#include <phxpr/primitives/actor.hpp>

#include <boost/spirit/home/prana/support/utree_predicates.hpp> // deprecated 

namespace phxpr {

struct procedure: actor<procedure> {
  utree body;

  procedure (void): body() { }

  procedure (utree const& body_):
    body(body_) { }

  procedure (procedure const& other):
    body(other.body) { }

  template <typename F>
  procedure (F const& body_):
    body(boost::spirit::stored_function<F>(body_)) { }

  bool empty (void) const {
    return prana::recursive_which(body) == utree_type::invalid_type;
  }

  utree eval (scope const& env) const {
    if (body.which() == utree_type::function_type)
      return body.eval(env);
    else
      return utree(boost::ref(body));
  }
};

} // phxpr

#endif // PHXPR_9E9F37AF_C618_4FD0_A51B_559364CF9827

