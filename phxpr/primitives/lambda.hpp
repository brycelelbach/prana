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

#include <phxpr/gc/make_shared.hpp>
#include <phxpr/signature.hpp>
#include <phxpr/primitives/procedure.hpp>

namespace phxpr {

struct lambda: actor<lambda> {
  typedef sheol::adt::dynamic_array<signature> gpt_type;

  phxpr::shared_ptr<function_body> body;
  phxpr::shared_ptr<gpt_type> global_procedure_table;
  const signature sig;
  
  lambda (function_body const& body_, phxpr::shared_ptr<gpt_type> const& gpt,
          signature const& sig_):
    body(phxpr::make_shared<function_body>(body_)), global_procedure_table(gpt),
    sig(sig_)
  {
    BOOST_ASSERT(gpt);
    BOOST_ASSERT(body);
  }

  lambda (phxpr::shared_ptr<function_body> const& body_, 
          phxpr::shared_ptr<gpt_type> const& gpt, signature const& sig_):
    body(body_), global_procedure_table(gpt), sig(sig_) 
  {
    BOOST_ASSERT(gpt);
    BOOST_ASSERT(body);
  }
  
  utree eval (utree& ut) const {
    using boost::fusion::at_c;

    BOOST_ASSERT(body);
    
    runtime_environment& env = *ut.get<runtime_environment*>();

    phxpr::shared_ptr<runtime_environment> saved_env;

    if (env.level() == 0)
      saved_env = env.checkout();
    else
      saved_env = env.outer();
       
    const signature proc_sig(at_c<0>(sig), at_c<1>(sig), at_c<2>(sig),
                             function_type::procedure, at_c<4>(sig));

    function_base* pf = new procedure
      (body, saved_env, global_procedure_table, proc_sig);

    return utree(pf); 
  }

  function_base* copy (void) const
  { return new lambda(body, global_procedure_table, sig); }
};

} // phxpr

#endif // PHXPR_F580B218_70FD_42EE_991F_52BA7814C51C

