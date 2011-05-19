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

  thunk (boost::shared_ptr<lazy_call_type> const& lazy_call_
         boost::shared_ptr<gpt_type> const& gpt):         
    lazy_call(lazy_call_), global_procedure_table(gpt),
  {
    BOOST_ASSERT(gpt);
    BOOST_ASSERT(lazy_call);
    BOOST_ASSERT(lazy_call->size() > 0);
  }

  // TODO: Optimize for the nullary case.
  utree eval (scope const& args) const {
    using boost::fusion::at_c;
    
    BOOST_ASSERT(lazy_call);
    BOOST_ASSERT(lazy_call->size() > 0);

    const displacement lazy_env_size = lazy_call.size();
    boost::shared_array<utree> lazy_env(new utree[lazy_env_size]);

    for (std::size_t i = 0, end = lazy_env_size; i != end; ++i) { 
      utree const& lazy_arg = (*lazy_call)[i];

      if (prana::recursive_which(lazy_arg) == utree_type::function_type) {
        BOOST_ASSERT(lazy_arg.tag() <= global_procedure_table->size());

        // Load the lazy argument's signature from the gpt.
        signature const& sig = (*global_procedure_table)[lazy_arg.tag()];

        if (at_c<3>(sig) == function_type::placeholder)
          lazy_env[i] = lazy_arg.eval(args);
        else
          // REVIEW: How safe is this?
          lazy_env[i] = boost::ref(lazy_arg);
      }

      else
        // REVIEW: How safe is this?
        lazy_env[i] = boost::ref(lazy_arg);
    }

    utree const& lazy_f = (*lazy_env)[0];

    if (prana::recursive_which(lazy_f) == utree_type::function_type) 
      BOOST_THROW_EXCEPTION(procedure_call_or_macro_use_expected(f));
 
    return lazy_f.eval(scope(lazy_env, lazy_env_size));
  }
};

} // phxpr

#endif // PHXPR_B75D9C09_6C20_431F_AFC6_0FDB283822B2

