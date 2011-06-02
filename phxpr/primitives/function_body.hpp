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

#include <boost/ref.hpp>

#include <sheol/adt/dynamic_array.hpp>

#include <prana/utree/predicates.hpp> 

#include <phxpr/gc/make_shared.hpp>
#include <phxpr/signature.hpp>

namespace phxpr {

struct function_body: actor<function_body> {
  typedef sheol::adt::dynamic_array<utree> code_type;

  phxpr::shared_ptr<code_type> code;

  function_body (utree const& code_): code(phxpr::make_shared<code_type>())
  { code->push_back(code_); }
  
  function_body (phxpr::shared_ptr<code_type> const& code_): code(code_) { }
 
//  function_body (function_body const& other): code(other.code) { }
 
  utree execute (utree const& element, runtime_environment& env) const { 
    using boost::fusion::at_c;

    if (prana::recursive_which(element) == utree_type::function_type)
      return env.invoke(element); 

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
  { return new function_body(code); }
};

} // phxpr

#endif // PHXPR_DB07A041_4FEF_48BA_8182_C8C02F6AA823

