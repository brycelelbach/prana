////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <phxpr/config.hpp>

#include <prana/utree/predicates.hpp>

#include <phxpr/evaluator.hpp>
#include <phxpr/exception.hpp>
  
using boost::fusion::at_c;

namespace phxpr {

void evaluator::make_placeholders (utree const& formals, signature const& sig) {
  // {{{
  utree::const_iterator it = formals.begin(), end = formals.end();

  for (utree::size_type i = 0, end = formals.size(); i != end; ++i, ++it) {
    if (prana::recursive_which(*it) != utree_type::symbol_type)
      BOOST_THROW_EXCEPTION(expected_identifier(*it)); 

    boost::shared_ptr<utree> p = variables->define(*it, utree
      (stored_function<placeholder>(placeholder(i, frame)))); 

    const signature ph_sig(i, ((end - 1) == i)
                              ? at_c<1>(sig)
                              : arity_type::fixed,
                           evaluation_strategy::call_by_value, 
                           function_type::placeholder);

    global_procedure_table->push_back(ph_sig);
    p->tag(global_procedure_table->size() - 1);
  }
} // }}}

evaluator::result_type
evaluator::make_lambda_expression (utree const& formals,
                                   evaluator::range_type const& body)
{ // {{{
  if (!prana::is_utree_container(formals))
    BOOST_THROW_EXCEPTION(expected_formals_list(formals));

  arity_type::info type = arity_type::fixed;

  if (!formals.empty() &&
      (formals.back() == utree(spirit::utf8_symbol_type("..."))))
    type = arity_type::variable;

  const signature sig(formals.size(), type, evaluation_strategy::call_by_value,
                      function_type::lambda);

  evaluator local_env(variables, global_procedure_table, frame + 1, sig);

  if (arity_type::variable == type) {
    utree range(evaluator::range_type
      (formals.begin(), --formals.end()), spirit::shallow);
    local_env.make_placeholders(range, sig);
  }

  else
    local_env.make_placeholders(formals, sig);

  boost::shared_ptr<function_body> fbody
    = boost::make_shared<function_body>
      (boost::make_shared<function_body::code_type>());

  typedef range_type::const_iterator iterator;

  iterator it = body.begin(), end = body.end();

  if (it == end)
    BOOST_THROW_EXCEPTION(expected_body(utree(body))); 

  for (; it != end; ++it) {
    utree f = local_env.make_thunk(*it, sig);
    fbody->code->push_back(f);
  }

  lambda l(fbody, local_env.global_procedure_table, sig);

  local_env.global_procedure_table->push_back(sig);

  utree ut = stored_function<lambda>(l);
  ut.tag(local_env.global_procedure_table->size() - 1);

  return ut; 
} // }}}

} // phxpr

