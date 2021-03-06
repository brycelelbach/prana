/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PHXPR_9E9F37AF_C618_4FD0_A51B_559364CF9827)
#define PHXPR_9E9F37AF_C618_4FD0_A51B_559364CF9827

#include <phxpr/config.hpp>

#include <boost/ref.hpp>

#include <prana/utree/predicates.hpp>

#include <phxpr/gc/shared_ptr.hpp>
#include <phxpr/exception.hpp>
#include <phxpr/signature.hpp>
#include <phxpr/primitives/actor.hpp>
#include <phxpr/primitives/function_body.hpp>

namespace phxpr {

struct procedure: actor<procedure> {
  typedef sheol::adt::dynamic_array<signature> gpt_type;

  phxpr::shared_ptr<function_body> body;
  phxpr::shared_ptr<runtime_environment> parent_env;
  phxpr::shared_ptr<gpt_type> global_procedure_table;
  const signature sig;

  procedure (phxpr::shared_ptr<function_body> const& body_,
             phxpr::shared_ptr<runtime_environment> const& parent_env_,
             phxpr::shared_ptr<gpt_type> const& gpt, signature const& sig_):
    body(body_), parent_env(parent_env_), global_procedure_table(gpt), sig(sig_)
  {
    BOOST_ASSERT(gpt);
    BOOST_ASSERT(body);
  }

  // TODO: Refactor this code with the code in thunk
  utree expand (utree const& arg, runtime_environment& env) const {
    using boost::fusion::at_c;

    if (prana::recursive_which(arg) == utree_type::function_type) {
      BOOST_ASSERT(arg.tag() <= global_procedure_table->size());

      // Load the argument's signature from the gpt.
      signature const& sig = (*global_procedure_table)[arg.tag()];

//      std::cout << at_c<3>(sig) << std::endl;

      if ((at_c<3>(sig) == function_type::placeholder)/*||
          (at_c<3>(sig) == function_type::reference)*/)
        return env.invoke(arg);
    }

    return arg;
  }

  utree eval (utree& ut) const {
    using boost::fusion::at_c;
    
    runtime_environment& env = *ut.get<runtime_environment*>();

#if 0
    // {{{ arity checking (TODO: variable arity)
    if (at_c<1>(sig) == arity_type::fixed) {
      if (at_c<0>(sig) != env.size())
        BOOST_THROW_EXCEPTION
          (invalid_arity(at_c<0>(sig), env.size(), arity_type::fixed));
    }

    else
      BOOST_THROW_EXCEPTION(unsupported_arity_type(at_c<1>(sig)));
    // }}}
#endif

    if (env.size() != 0) {
      if (at_c<4>(sig) != 0) {
        // {{{ environment extension for local variables
        const displacement ext_env_size = env.size() + at_c<4>(sig);

        phxpr::shared_ptr<runtime_environment::upvalue_type> storage
          = phxpr::make_shared<runtime_environment::upvalue_type>
            (ext_env_size, sheol::fill);

        // Extend the environment.
        for (std::size_t i = 0, end = env.size(); i != end; ++i)
          (*storage)[i] = env[i];
        
        phxpr::shared_ptr<runtime_environment> new_env
          = phxpr::make_shared<runtime_environment>(storage, parent_env);
        return new_env->invoke(body);
      } // }}}

      phxpr::shared_ptr<runtime_environment::upvalue_type> storage
        = phxpr::make_shared<runtime_environment::upvalue_type>
          (env.size(), sheol::fill);

      for (std::size_t i = 0, end = env.size(); i != end; ++i)
        (*storage)[i] = expand(env[i], env);

      phxpr::shared_ptr<runtime_environment> new_env
        = phxpr::make_shared<runtime_environment>(storage, parent_env);
      return new_env->invoke(body);
    }
  
    // nullary  
    else { 
      if (at_c<4>(sig) != 0) {
        phxpr::shared_ptr<runtime_environment::upvalue_type> storage
          = phxpr::make_shared<runtime_environment::upvalue_type>
            (at_c<4>(sig), sheol::fill);

        // Extend the environment.
        for (std::size_t i = 0, end = env.size(); i != end; ++i)
          (*storage)[i] = env[i];
        
        phxpr::shared_ptr<runtime_environment> new_env
          = phxpr::make_shared<runtime_environment>(storage, parent_env);
        return new_env->invoke(body);
      } // }}}

      phxpr::shared_ptr<runtime_environment> new_env
        = phxpr::make_shared<runtime_environment>(parent_env);
      return new_env->invoke(body);
    }
  }

  function_base* copy (void) const
  { return new procedure(body, parent_env, global_procedure_table, sig); }
};

} // phxpr

#endif // PHXPR_9E9F37AF_C618_4FD0_A51B_559364CF9827

