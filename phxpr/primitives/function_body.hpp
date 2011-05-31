/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PHXPR_DB07A041_4FEF_48BA_8182_C8C02F6AA823)
#define PHXPR_DB07A041_4FEF_48BA_8182_C8C02F6AA823

#include <phxpr/config.hpp>

#include <boost/make_shared.hpp>
#include <boost/ref.hpp>

#include <sheol/adt/dynamic_array.hpp>

#include <prana/utree/predicates.hpp> 

#include <phxpr/signature.hpp>
#include <phxpr/primitives/actor.hpp>

namespace phxpr {

struct function_body: actor<function_body> {
  typedef sheol::adt::dynamic_array<utree> code_type;
  typedef sheol::adt::dynamic_array<signature> gpt_type;

  boost::shared_ptr<code_type> code;
  boost::shared_ptr<gpt_type> global_procedure_table;

  // {{{ REVIEW: How many of these constructors do we actually need?
  function_body (utree const& code_, boost::shared_ptr<gpt_type> const& gpt):
    code(boost::make_shared<code_type>()), global_procedure_table(gpt)
  { code->push_back(code_); }
  
  function_body (boost::shared_ptr<code_type> const& code_,
                 boost::shared_ptr<gpt_type> const& gpt):
    code(code_), global_procedure_table(gpt)
  { }

  function_body (function_body const& other):
    code(other.code), global_procedure_table(other.global_procedure_table)
  { }

//  template <typename F>
//  function_body (F const& code_): code(boost::make_shared<code_type>())
//  { code->push_back(utree(stored_function<F>(code_))); }
  // }}}
  
  // TODO: Refactor this code with the code in procedure, thunk and others
  utree execute (utree const& element, runtime_environment& env) const { 
    using boost::fusion::at_c;

    if (prana::recursive_which(element) == utree_type::function_type) {
      BOOST_ASSERT(element.tag() <= global_procedure_table->size());

      // Load the element argument's signature from the gpt.
      signature const& sig = (*global_procedure_table)[element.tag()];

//      std::cout << at_c<3>(sig) << std::endl;

//      if (at_c<3>(sig) == function_type::thunk)
//        return execute(env.invoke(element), env);
//        return env;
//      else
      return env.invoke(element); 
    }

    return utree(boost::ref(element));
  }

  utree eval (utree& ut) const {
    BOOST_ASSERT(code);
    BOOST_ASSERT(code->size());
    
    runtime_environment& env = *ut.get<runtime_environment*>();

    const code_type::size_type end = code->size();

    for (code_type::size_type i = 0; i != (end - 1); ++i)
      execute((*code)[i], env);

    return execute((*code)[end - 1], env);
  }

  // REVIEW: Will passing shallow references via smart pointers screw with
  // assignment once it's implemented?
  function_base* copy (void) const
  { return new function_body(code, global_procedure_table); }
};

} // phxpr

#endif // PHXPR_DB07A041_4FEF_48BA_8182_C8C02F6AA823

