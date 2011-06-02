////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <phxpr/config.hpp>

#include <iterator>

#include <boost/assert.hpp>

#include <prana/utree/predicates.hpp>

#include <phxpr/evaluator.hpp>
#include <phxpr/predicate.hpp>
#include <phxpr/exception.hpp>
  
using boost::fusion::at_c;

namespace phxpr {

evaluator::result_type
evaluator::execute (evaluator::range_type const& range) {
  // {{{
  typedef evaluator::range_type::const_iterator iterator;

  iterator it = range.begin(), end = range.end();

  if (it == end)
    BOOST_THROW_EXCEPTION(invalid_operator_expression(utree(range))); 

  utree f;

  // Evaluate the operator expresion.
  if (prana::is_utree_container(*it)) {
    f = evaluate(*it, *this);

    if (prana::recursive_which(f) != utree_type::function_type)
      BOOST_THROW_EXCEPTION(procedure_call_or_macro_use_expected(*it));
  }

  // Evaluate the named operator.
  else if (prana::recursive_which(*it) == utree_type::symbol_type) {
    // TODO: syntax checks
    if (*it == utree(spirit::utf8_symbol_type("lambda"))) {
      iterator formals = ++it; 
      iterator body = ++it; 
      f = make_lambda_expression(*formals, range_type(body, end)); 
      phxpr::shared_ptr<runtime_environment> new_env
        = phxpr::make_shared<runtime_environment>();
      return new_env->invoke(f);
    }
    
    else if (*it == utree(spirit::utf8_symbol_type("variable"))) {
      iterator identifier = ++it;
      iterator value = ++it; 

      if (++it != end)
        BOOST_THROW_EXCEPTION(invalid_variable_definition
          (utree(range_type(range.begin(), value)))); 
       
      // Module-level variable definition.
      BOOST_ASSERT(frame == 0);
      return make_module_level_variable(*identifier, *value); 
    }
    
    // TODO: syntax checks
    else if (*it == utree(spirit::utf8_symbol_type("if"))) {
      iterator test = ++it; 
      iterator then = ++it; 
      iterator else_ = ++it;

      bool r = predicate(evaluate(*test, *this));

      if (r)
        return evaluate(*then, *this);

      // (if <false> <then>) returns unspecified (aka invalid)
      if (else_ == end)
        return utree(); 

      return evaluate(*else_, *this);
    }
    
    else {
      phxpr::shared_ptr<utree> pf = variables->lookup(*it);

      if (!pf)
        BOOST_THROW_EXCEPTION(identifier_not_found(*it));

      f = *pf;
    }
  }

  else 
    BOOST_THROW_EXCEPTION(procedure_call_or_macro_use_expected(*it));
      
  BOOST_ASSERT(unsigned(f.tag()) <= global_procedure_table->size());

  // Load the lazy argument's signature from the gpt.
  signature const& sig = (*global_procedure_table)[f.tag()];

  if ((at_c<3>(sig) == function_type::thunk) ||
      (at_c<3>(sig) == function_type::lambda))
  {
    phxpr::shared_ptr<runtime_environment> new_env
      = phxpr::make_shared<runtime_environment>();
    f = new_env->invoke(f);
  }

  // Invoke nullary procedures.
  if (++it == end) {
    phxpr::shared_ptr<runtime_environment> new_env
      = phxpr::make_shared<runtime_environment>();
    return new_env->invoke(f);
  }

  // Invoke non-nullary procedures.
  const displacement env_size = at_c<4>(sig) + std::distance(it, end);  
  phxpr::shared_array<utree> env(new utree[env_size]);

  // Evaluate the arguments.
  for (std::size_t i = 0; it != end; ++it)
    env[i++] = evaluate(*it, *this);

  phxpr::shared_ptr<runtime_environment> new_env
    = phxpr::make_shared<runtime_environment>(env, env_size);
  return new_env->invoke(f);
} // }}}

} // phxpr

