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

#include <boost/shared_ptr.hpp>

#include <phxpr/signature.hpp>
#include <phxpr/primitives/function_body.hpp>
#include <phxpr/primitives/procedure.hpp>

namespace phxpr {

struct lambda: actor<lambda> {
  boost::shared_ptr<function_body> body;
  signature sig;
  displacement num_locals;

  lambda (boost::shared_ptr<function_body> const& body_, signature const& sig_,
          displacement num_locals_)
    body(body_), sig(sig_), num_locals(num_locals_)
  { BOOST_ASSERT(body); }
  
  utree eval (scope const& env) const {
    BOOST_ASSERT(body);

    function_base* pf = new stored_function<procedure>
      (body, sig, num_locals, env.checkout());

    return utree(pf); 
  }
};

} // phxpr

#endif // PHXPR_F580B218_70FD_42EE_991F_52BA7814C51C

