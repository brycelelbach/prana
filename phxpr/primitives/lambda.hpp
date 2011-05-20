/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PHXPR_F580B218_70FD_42EE_991F_52BA7814C51C)
#define PHXPR_F580B218_70FD_42EE_991F_52BA7814C51C

#include <phxpr/config.hpp>

#include <boost/make_shared.hpp>

#include <phxpr/signature.hpp>
#include <phxpr/primitives/procedure.hpp>

namespace phxpr {

struct lambda: actor<lambda> {
  boost::shared_ptr<function_body> body;
  signature sig;
  displacement num_local_vars;
  
  lambda (function_body const& body_, signature const& sig_,
          displacement num_local_vars_ = 0):
    body(boost::make_shared<function_body>(body_)), sig(sig_),
    num_local_vars(num_local_vars_)
  { BOOST_ASSERT(body); }

  lambda (boost::shared_ptr<function_body> const& body_, signature const& sig_,
          displacement num_local_vars_ = 0):
    body(body_), sig(sig_), num_local_vars(num_local_vars_)
  { BOOST_ASSERT(body); }
  
  utree eval (scope const& env) const {
    using boost::fusion::at_c;

    BOOST_ASSERT(body);

    boost::shared_ptr<scope> saved_env;

    if (env.level() == 0)
      saved_env = env.get();
    else
      saved_env = env.outer();

    function_base* pf = new stored_function<procedure>
      (procedure(body, saved_env, signature
        (at_c<0>(sig), at_c<1>(sig), at_c<2>(sig), function_type::procedure),
          num_local_vars));

    return utree(pf); 
  }
};

} // phxpr

#endif // PHXPR_F580B218_70FD_42EE_991F_52BA7814C51C

