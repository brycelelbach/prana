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
#include <phxpr/primitives/conditional.hpp>
#include <phxpr/primitives/thunk.hpp>
#include <phxpr/primitives/assignment.hpp>
#include <phxpr/primitives/reference.hpp>
  
using boost::fusion::at_c;

namespace phxpr {

evaluator::result_type
evaluator::make_thunk (utree const& elements, signature const& sig) {
  // {{{
  const signature body_sig(at_c<0>(sig), at_c<1>(sig), at_c<2>(sig), 
                           function_type::thunk, at_c<4>(sig));

  phxpr::shared_ptr<thunk::lazy_call_type> lazy_call
    = phxpr::make_shared<thunk::lazy_call_type>();

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
      lazy_call->push_back(make_lambda_expression
        (*formals, evaluator::range_type(body, end))); 
    }

    // TODO: syntax checks
    // TODO: bad define placement checks
    else if (*it == utree(spirit::utf8_symbol_type("variable"))) {
      iterator identifier = ++it; 
      iterator value = ++it; 
      lazy_call->push_back(make_internal_variable(*identifier, *value)); 
    }
    
    // TODO: syntax checks
    else if (*it == utree(spirit::utf8_symbol_type("if"))) {
      iterator test= ++it; 
      iterator then = ++it;
      iterator else_ = ++it;
      if (else_ == end)
        lazy_call->push_back(make_if_thunk(*test, *then)); 
      else
        lazy_call->push_back(make_if_else_thunk(*test, *then, *else_)); 
    }

    else {
      BOOST_FOREACH(utree const& element, elements) {
        if (prana::is_utree_container(element))
          lazy_call->push_back(make_thunk(element, sig));
        else
//          lazy_call->push_back(make_variable_reference_thunk(element));
          lazy_call->push_back(evaluate(element, *this));
      }
    }
  }

  else
    lazy_call->push_back(make_variable_reference_thunk(elements)); 
    
  utree ut = new thunk(lazy_call, global_procedure_table);
  global_procedure_table->push_back(body_sig);
  ut.tag(global_procedure_table->size() - 1);

  return ut;
} // }}}

// TODO: Optimize for test and/or then being a literal.
evaluator::result_type
evaluator::make_if_thunk (utree const& test, utree const& then) {
  // {{{
  const signature sig(2, arity_type::fixed, evaluation_strategy::call_by_value, 
                      function_type::conditional, 0);

  utree ut = new phxpr::if_(make_thunk(test, sig),
                            make_thunk(then, sig));

  global_procedure_table->push_back(sig);
  ut.tag(global_procedure_table->size() - 1);

  return ut;
} // }}}

// TODO: Optimize for test, then and/or else_ being a literal.
evaluator::result_type
evaluator::make_if_else_thunk (utree const& test, utree const& then,
                               utree const& else_)
{ // {{{
  const signature sig(3, arity_type::fixed, evaluation_strategy::call_by_value, 
                      function_type::conditional, 0);

  utree ut = new phxpr::if_else(make_thunk(test, sig),
                                make_thunk(then, sig),
                                make_thunk(else_, sig));

  global_procedure_table->push_back(sig);
  ut.tag(global_procedure_table->size() - 1);

  return ut;
} // }}}

// TODO: Optimize for value being a literal. 
evaluator::result_type
evaluator::make_internal_variable_thunk (utree const& value, displacement n,
                                         displacement frame) 
{ // {{{
  const signature sig(n, arity_type::fixed, evaluation_strategy::call_by_value, 
                      function_type::definition, 0);

  utree ut = new local_assignment(make_thunk(value, sig), n, frame);

  global_procedure_table->push_back(sig);
  ut.tag(global_procedure_table->size() - 1);

  return ut;
} // }}}

// TODO: Optimize for value being a datum. 
evaluator::result_type
evaluator::make_variable_reference_thunk (utree const& datum) {
  // {{{
  utree value = evaluate(datum, *this);
  
  signature sig(0, arity_type::fixed, evaluation_strategy::call_by_value, 
                function_type::reference, 0);

  if (prana::recursive_which(value) == utree_type::function_type) {
    BOOST_ASSERT(unsigned(value.tag()) <= global_procedure_table->size());
    sig = (*global_procedure_table)[value.tag()];
    at_c<3>(sig) = function_type::reference; 
  }

  utree r = new variable_reference(value, global_procedure_table);
  global_procedure_table->push_back(sig);
  r.tag(global_procedure_table->size() - 1);

  return r;
} // }}}

} // phxpr

