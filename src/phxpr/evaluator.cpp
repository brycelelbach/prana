////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <phxpr/config.hpp>

#include <iterator>

#include <boost/assert.hpp>

#include <prana/utree/io.hpp>
#include <prana/dispatch/visit_ref.hpp>
#include <prana/utree/predicates.hpp>

#include <phxpr/evaluator.hpp>
#include <phxpr/predicate.hpp>
#include <phxpr/exception.hpp>
#include <phxpr/primitives/conditional.hpp>
#include <phxpr/primitives/placeholder.hpp>
#include <phxpr/primitives/thunk.hpp>

namespace phxpr {

// {{{ internal evaluator algorithms
evaluator::result_type
evaluate_lambda_expression (utree const& formals,
                            evaluator::range_type const& body, evaluator& ev);

evaluator::result_type
make_if_thunk (utree const& test, utree const& then, evaluator& ev);

evaluator::result_type
make_if_else_thunk (utree const& test, utree const& then, utree const& else_,
                    evaluator& ev);
 
evaluator::result_type
evaluate_module_level_variable (utree const& identifier, utree const& value,
                                evaluator& ev)
{
  boost::shared_ptr<utree> p(ev.variables->declare(identifier));
  *p = evaluate(value, ev);
  // return value of a definition is unspecified (aka invalid utree)
  return utree();
}

evaluator::result_type
evaluate_internal_variable (utree const& identifier, utree const& value,
                            evaluator& ev) 
{
  // IMPLEMENT
  return utree();
}

evaluator::result_type
make_thunk (utree const& elements, signature const& sig, evaluator& ev) {
  using boost::fusion::at_c;

  const signature body_sig(at_c<0>(sig), at_c<1>(sig), at_c<2>(sig), 
                           function_type::thunk);

  boost::shared_ptr<thunk::lazy_call_type> lazy_call
    = boost::make_shared<thunk::lazy_call_type>();

  if (prana::is_utree_container(elements)) {
    typedef utree::const_iterator iterator;

    iterator it = elements.begin(), end = elements.end();

    if (it == end) {
      BOOST_THROW_EXCEPTION(procedure_call_or_macro_use_expected(elements));
    }

    // TODO: syntax checks
    else if (*it == utree(spirit::utf8_symbol_type("lambda"))) {
      iterator formals = ++it;
      iterator body = ++it; 
      ++it;
      lazy_call->push_back(evaluate_lambda_expression
        (*formals, evaluator::range_type(body, end), ev)); 
    }

    // TODO: syntax checks
    else if (*it == utree(spirit::utf8_symbol_type("variable"))) {
      iterator identifier = ++it; 
      iterator value = ++it; 
      lazy_call->push_back(evaluate_internal_variable(*identifier, *value, ev)); 
    }
    
    // TODO: syntax checks
    else if (*it == utree(spirit::utf8_symbol_type("if"))) {
      iterator test= ++it; 
      iterator then = ++it;
      iterator else_ = ++it;
      if (else_ == end)
        lazy_call->push_back(make_if_thunk(*test, *then, ev)); 
      else
        lazy_call->push_back(make_if_else_thunk(*test, *then, *else_, ev)); 
    }

    else {
      BOOST_FOREACH(utree const& element, elements) {
        if (prana::is_utree_container(element))
          lazy_call->push_back(make_thunk(element, sig, ev));
        else
          lazy_call->push_back(evaluate(element, ev));
      }
    }
  }

  else
    lazy_call->push_back(evaluate(elements, ev));
    
  thunk t(lazy_call, ev.global_procedure_table);

  ev.global_procedure_table->push_back(body_sig);

  utree ut = stored_function<thunk>(t);
  ut.tag(ev.global_procedure_table->size() - 1);

  return ut;
}

// TODO: Optimize for test and/or then being a literal.
evaluator::result_type
make_if_thunk (utree const& test, utree const& then, evaluator& ev) {
  const signature sig(2, arity_type::fixed, evaluation_strategy::call_by_value, 
                      function_type::conditional);

  phxpr::if_ c(make_thunk(test, sig, ev), make_thunk(then, sig, ev));

  ev.global_procedure_table->push_back(sig);

  utree ut = stored_function<phxpr::if_>(c);
  ut.tag(ev.global_procedure_table->size() -1);

  return ut;
}

// TODO: Optimize for test, then and/or else_ being a literal.
evaluator::result_type
make_if_else_thunk (utree const& test, utree const& then, utree const& else_,
                    evaluator& ev)
{
  const signature sig(3, arity_type::fixed, evaluation_strategy::call_by_value, 
                      function_type::conditional);

  phxpr::if_else c(make_thunk(test, sig, ev), make_thunk(then, sig, ev),
    make_thunk(else_, sig, ev));

  ev.global_procedure_table->push_back(sig);

  utree ut = stored_function<phxpr::if_else>(c);
  ut.tag(ev.global_procedure_table->size() -1);

  return ut;
}

// TODO: handle variable arguments
void make_placeholders (utree const& formals, signature const& sig,
                        evaluator& ev)
{
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
    p->tag(ev.global_procedure_table->size() - 1);
  }
}

evaluator::result_type
evaluate_lambda_expression (utree const& formals,
                            evaluator::range_type const& body, evaluator& ev)
{
  if (!prana::is_utree_container(formals))
    BOOST_THROW_EXCEPTION(expected_formals_list(formals));

  arity_type::info type = arity_type::fixed;

  if (!formals.empty() &&
      (formals.back() == utree(spirit::utf8_symbol_type("..."))))
    type = arity_type::variable;

  const signature sig(formals.size(), type, evaluation_strategy::call_by_value,
                      function_type::lambda);

  evaluator local_env
    (ev.variables, ev.global_procedure_table, ev.frame + 1, sig);

  if (arity_type::variable == type) {
    utree range(evaluator::range_type
      (formals.begin(), --formals.end()), spirit::shallow);
    make_placeholders(range, sig, local_env);
  }

  else
    make_placeholders(formals, sig, local_env);

  boost::shared_ptr<function_body> fbody
    = boost::make_shared<function_body>
      (boost::make_shared<function_body::code_type>());

  typedef evaluator::range_type::const_iterator iterator;

  iterator it = body.begin(), end = body.end();

  if (it == end)
    BOOST_THROW_EXCEPTION(expected_body(utree(body))); 

  for (; it != end; ++it) {
    utree f = make_thunk(*it, sig, local_env);
    fbody->code->push_back(f);
  }

  lambda l(fbody, local_env.global_procedure_table, sig);

  local_env.global_procedure_table->push_back(sig);

  utree ut = stored_function<lambda>(l);
  ut.tag(local_env.global_procedure_table->size() - 1);

  return ut; 
}
// }}}

evaluator::result_type
evaluator::operator() (evaluator::symbol_type const& str) {
  boost::shared_ptr<utree> p = variables->lookup(utree(str));

  if (!p)
    BOOST_THROW_EXCEPTION(identifier_not_found(utree(str)));

  return *p;
}

evaluator::result_type
evaluator::operator() (evaluator::range_type const& range) {
  using boost::fusion::at_c;

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
      f = evaluate_lambda_expression(*formals, range_type(body, end), *this); 
      boost::shared_ptr<runtime_environment> new_env
        = boost::make_shared<runtime_environment>();
      return new_env->invoke(f);
    }
    
    else if (*it == utree(spirit::utf8_symbol_type("variable"))) {
      iterator identifier = ++it;
      iterator value = ++it; 

      if (++it != end)
        BOOST_THROW_EXCEPTION(invalid_variable_definition
          (utree(range_type(range.begin(), value)))); 
       
      // Module-level variable definition.
      if (frame == 0)
        return evaluate_module_level_variable(*identifier, *value, *this); 
      // Internal variable definition.
      else
        return evaluate_internal_variable(*identifier, *value, *this); 
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
      boost::shared_ptr<utree> pf = variables->lookup(*it);

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
    boost::shared_ptr<runtime_environment> new_env
      = boost::make_shared<runtime_environment>();
    f = new_env->invoke(f);
  }

  // Invoke nullary procedures.
  if (++it == end) {
    boost::shared_ptr<runtime_environment> new_env
      = boost::make_shared<runtime_environment>();
    return new_env->invoke(f);
  }

  // Invoke non-nullary procedures.
  const displacement env_size = num_local_variables + std::distance(it, end);  
  boost::shared_array<utree> env(new utree[env_size]);

  // Evaluate the arguments.
  for (std::size_t i = 0; it != end; ++it)
    env[i++] = evaluate(*it, *this);

  boost::shared_ptr<runtime_environment> new_env
    = boost::make_shared<runtime_environment>(env, env_size);
  return new_env->invoke(f);
}

// {{{ evaluate 
utree evaluate (sexpr_parse_tree const& pt) {
  return evaluate(pt.ast());
}

utree evaluate (sexpr_parse_tree const& pt, evaluator& ev) {
  return evaluate(pt.ast(), ev);
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

