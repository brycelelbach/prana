/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PHXPR_B75D9C09_6C20_431F_AFC6_0FDB283822B2)
#define PHXPR_B75D9C09_6C20_431F_AFC6_0FDB283822B2

#include <phxpr/config.hpp>

#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#include <prana/utree/predicates.hpp>

#include <phxpr/exception.hpp>
#include <phxpr/signature.hpp>
#include <phxpr/primitives/actor.hpp>
#include <phxpr/primitives/function_body.hpp>

namespace phxpr {

struct thunk: actor<thunk> {
  typedef sheol::adt::dynamic_array<utree> lazy_call_type;
  typedef sheol::adt::dynamic_array<signature> gpt_type;

  boost::shared_ptr<lazy_call_type> lazy_call;
  boost::shared_ptr<gpt_type> global_procedure_table;

  thunk (boost::shared_ptr<lazy_call_type> const& lazy_call_,
         boost::shared_ptr<gpt_type> const& gpt):   
    lazy_call(lazy_call_), global_procedure_table(gpt)
  {
    BOOST_ASSERT(gpt);
    BOOST_ASSERT(lazy_call);
    BOOST_ASSERT(lazy_call->size() > 0);
  }

  // TODO: Refactor this code with the code in procedure
  utree execute_lazy (utree& lazy, runtime_environment& env) const { 
    using boost::fusion::at_c;

    if (prana::recursive_which(lazy) == utree_type::function_type) {
      BOOST_ASSERT(lazy.tag() <= global_procedure_table->size());

      // Load the lazy argument's signature from the gpt.
      signature const& sig = (*global_procedure_table)[lazy.tag()];

      if ((at_c<3>(sig) == function_type::placeholder) ||
          (at_c<3>(sig) == function_type::thunk) ||
          (at_c<3>(sig) == function_type::reference))
        return env.invoke(lazy);
    }

    return lazy;
  }

  utree eval (utree& ut) const {
    BOOST_ASSERT(lazy_call);
    BOOST_ASSERT(lazy_call->size() > 0);

    runtime_environment& env = *ut.get<runtime_environment*>();

    utree const& lazy_f = execute_lazy((*lazy_call)[0], env);

    // IMPLEMENT: Allocate space for locals here (actually maybe not).
    const displacement lazy_env_size = lazy_call->size() - 1;
    boost::shared_array<utree> lazy_env(new utree[lazy_env_size]);

    for (std::size_t i = 0, end = lazy_env_size; i != end; ++i)
      lazy_env[i] = execute_lazy((*lazy_call)[i + 1], env);

    if (prana::recursive_which(lazy_f) != utree_type::function_type) 
      return lazy_f;

    boost::shared_ptr<runtime_environment> new_env
      = boost::make_shared<runtime_environment>
        (lazy_env, lazy_env_size, env.checkout());

    return new_env->invoke(lazy_f);
  }

  // REVIEW: Will passing shallow references via smart pointers screw with
  // assignment once it's implemented?
  function_base* copy (void) const
  { return new thunk(lazy_call, global_procedure_table); }
};

} // phxpr

#endif // PHXPR_B75D9C09_6C20_431F_AFC6_0FDB283822B2

