////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <iterator>

#include <prana/dispatch/visit_ref.hpp>
#include <prana/utree/predicates.hpp>
#include <phxpr/evaluator.hpp>
#include <phxpr/exception.hpp>
#include <phxpr/primitives/lambda.hpp>
#include <phxpr/primitives/placeholder.hpp>

namespace phxpr {

// {{{ internal evaluator algorithms
evaluator::result_type
evaluate_lambda_body (utree const& body, evaluator& ev) {
  // IMPLEMENT
  return evaluator::result_type();
}

// IMPLEMENT: handle variable arguments
void make_placeholder (utree const& formals, evaluator& ev) {
  typedef utree::const_iterator iterator;

  iterator it = formals.begin(), end = formals.end();

  for (utree::size_type i = 0, end = formals.size(); i != end; ++i, ++it) {
    if (prana::recursive_which(*it) != utree_type::symbol_type)
      BOOST_THROW_EXCEPTION(expected_identifier(*it)); 

    boost::shared_ptr<utree> p = ev.variables->define(*it, utree
      (stored_function<placeholder>(placeholder(i, ev.frame)))); 

    const signature sig(i, arity_type::fixed,
                        evaluation_strategy::call_by_value, 
                        function_type::placeholder);

    ev.global_procedure_table->push_back(sig);
    p->tag(ev.global_procedure_table->size());
  }
}

evaluator::result_type
evaluate_lambda_expression (utree const& formals,
                            evaluator::range_type const& body, evaluator& ev)
{
  evaluator local_env(ev.variables, ev.global_procedure_table, ev.frame + 1);

  if (!prana::is_utree_container(formals))
    BOOST_THROW_EXCEPTION(expected_formals_list(formals));

  arity_type::info type = arity_type::fixed;

  if (formals.back() == "...")
    type = arity_type::variable;

  const signature sig(formals.size(), type, evaluation_strategy::call_by_value,
                      function_type::lambda);

  make_placeholder(formals, local_env);

  boost::shared_ptr<function_body> fbody = boost::make_shared<function_body>();

  typedef evaluator::range_type::const_iterator iterator;

  iterator it = body.begin(), end = body.end();

  if (it != end)
    BOOST_THROW_EXCEPTION(expected_body(utree(body))); 

  for (; it != end; ++it) {
    utree f = evaluate_lambda_body(*it, local_env);
    fbody->code->push_back(f);
  }

  lambda l(fbody, sig);

  local_env.global_procedure_table->push_back(sig);

  utree ut = stored_function<lambda>(l);
  ut.tag(local_env.global_procedure_table->size());

  return ut; 
}
// }}}

evaluator::result_type
evaluator::operator() (evaluator::symbol_type const& str) {
  using boost::fusion::at_c;

  boost::shared_ptr<utree> p = variables->lookup(utree(str));

  if (!p)
    BOOST_THROW_EXCEPTION(identifier_not_found(utree(str)));

  return *p;
}

evaluator::result_type
evaluator::operator() (evaluator::range_type const& range) {
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
  if (prana::recursive_which(*it) == utree_type::symbol_type) {
    if (*it == "lambda") {
      iterator formals = it; ++formals;
      iterator body = formals; ++body; 
      return evaluate_lambda_expression(*formals, range_type(body, end), *this); 
    }

    else {
      boost::shared_ptr<utree> pf = variables->lookup(*it);

      if (!pf)
        BOOST_THROW_EXCEPTION(identifier_not_found(*it));

      f = *pf;
    }
  }

  else
    BOOST_THROW_EXCEPTION(procedure_call_or_macro_use_expected(*it));

  // Invoke nullary procedures.
  if (++it == end)
    return f.eval(scope());

  // Invoke non-nullary procedures.
  const displacement env_size = num_local_variables + std::distance(it, end);  
  boost::shared_array<utree> env(new utree[env_size]);

  // Evaluate the arguments.
  for (std::size_t i = 0; it != end; ++it)
    env[i++] = evaluate(*it, *this);

  return f.eval(scope(env, env_size));
}

// {{{ evaluate 
utree evaluate (sexpr_parse_tree const& pt) {
  return evaluate(pt.ast());
}

utree evaluate (utree const& ut) {
  evaluator ev;
  return evaluate(ut, ev);
}

utree evaluate (utree const& ut, evaluator& ev) {
  return utree::visit(ut, prana::visit_ref(ev));
}
// }}}

} // phxpr

