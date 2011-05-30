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
  
using boost::fusion::at_c;

namespace phxpr {

evaluator::result_type
evaluator::make_thunk (utree const& elements, signature const& sig) {
  // {{{
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
      lazy_call->push_back(make_lambda_expression
        (*formals, evaluator::range_type(body, end))); 
    }

    // TODO: syntax checks
    else if (*it == utree(spirit::utf8_symbol_type("variable"))) {
      iterator identifier = ++it; 
      iterator value = ++it; 
      lazy_call->push_back(make_internal_variable
        (*identifier, *value, sig)); 
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
          lazy_call->push_back(evaluate(element, *this));
      }
    }
  }

  else
    lazy_call->push_back(evaluate(elements, *this));
    
  thunk t(lazy_call, global_procedure_table);

  global_procedure_table->push_back(body_sig);

  utree ut = stored_function<thunk>(t);
  ut.tag(global_procedure_table->size() - 1);

  return ut;
} // }}}

// TODO: Optimize for test and/or then being a literal.
evaluator::result_type
evaluator::make_if_thunk (utree const& test, utree const& then) {
  // {{{
  const signature sig(2, arity_type::fixed, evaluation_strategy::call_by_value, 
                      function_type::conditional);

  phxpr::if_ c(make_thunk(test, sig),
               make_thunk(then, sig));

  global_procedure_table->push_back(sig);

  utree ut = stored_function<phxpr::if_>(c);
  ut.tag(global_procedure_table->size() -1);

  return ut;
} // }}}

// TODO: Optimize for test, then and/or else_ being a literal.
evaluator::result_type
evaluator::make_if_else_thunk (utree const& test, utree const& then,
                               utree const& else_)
{ // {{{
  const signature sig(3, arity_type::fixed, evaluation_strategy::call_by_value, 
                      function_type::conditional);

  phxpr::if_else c(make_thunk(test, sig),
                   make_thunk(then, sig),
                   make_thunk(else_, sig));

  global_procedure_table->push_back(sig);

  utree ut = stored_function<phxpr::if_else>(c);
  ut.tag(global_procedure_table->size() -1);

  return ut;
} // }}}

} // phxpr

