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
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#include <phxpr/exception.hpp>
#include <phxpr/signature.hpp>
#include <phxpr/primitives/actor.hpp>
#include <phxpr/primitives/function_body.hpp>

namespace phxpr {

struct procedure: actor<procedure> {
  boost::shared_ptr<function_body> body;
  boost::shared_ptr<scope> parent_env;
  signature sig;
  displacement num_local_vars;

  procedure (boost::shared_ptr<function_body> const& body_,
             boost::shared_ptr<scope> const& parent_env_, signature const& sig_,
             displacement num_local_vars_):
    body(body_), parent_env(parent_env_), sig(sig_),
    num_local_vars(num_local_vars_)
  { BOOST_ASSERT(body); }

  utree eval (scope const& args) const {
    using boost::fusion::at_c;

    // {{{ arity checking (TODO: variable arity)
    if (at_c<1>(sig) == arity_type::fixed) {
      if (at_c<0>(sig) != args.size())
        BOOST_THROW_EXCEPTION
          (invalid_arity(at_c<0>(sig), args.size(), arity_type::fixed));
    }

    else
      BOOST_THROW_EXCEPTION(unsupported_arity_type(at_c<1>(sig)));
    // }}}

    if (args.size() != 0) {
      if (num_local_vars != 0) {
        // {{{ environment extension for local variables
        const displacement ext_env_size = args.size() + num_local_vars;

        // TODO: Implement make_shared_array<>.
        boost::shared_array<utree> ext_env(new utree[ext_env_size]);

        // Extend the environment.
        for (std::size_t i = 0, end = args.size(); i != end; ++i)
          ext_env[i] = args[i];
        
        boost::shared_ptr<scope> new_scope = boost::make_shared<scope>
          (ext_env, ext_env_size, parent_env);
        return body->eval(*new_scope);
      } // }}}

      boost::shared_array<utree> const& ap = args.checkout();

      // fastpath
      if (ap) { 
        boost::shared_ptr<scope> new_scope = boost::make_shared<scope>
          (ap, args.size(), parent_env);
        return body->eval(*new_scope);
      }

      // slowpath (TODO: shouldn't ever happen)
      else {
        boost::shared_array<utree> storage(new utree[args.size()]);

        for (std::size_t i = 0, end = args.size(); i != end; ++i)
          storage[i] = args[i];

        boost::shared_ptr<scope> new_scope = boost::make_shared<scope>
          (storage, args.size(), parent_env);
        return body->eval(*new_scope);
      }
    }
  
    // nullary  
    else { 
      boost::shared_ptr<scope> new_scope
        = boost::make_shared<scope>(parent_env);
      return body->eval(*new_scope);
    }
  }
};

} // phxpr

#endif // PHXPR_9E9F37AF_C618_4FD0_A51B_559364CF9827

