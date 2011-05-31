////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_8A7BB292_8252_42D4_AEC4_F7F827B95E68)
#define PHXPR_8A7BB292_8252_42D4_AEC4_F7F827B95E68

#include <phxpr/config.hpp>

#include <boost/noncopyable.hpp>

#include <sheol/adt/dynamic_array.hpp>

#include <prana/utree/hash.hpp>
#include <prana/parse/parse_tree.hpp> 
#include <prana/parse/grammar/sexpr.hpp>

#include <phxpr/environment.hpp>
#include <phxpr/signature.hpp>
#include <phxpr/primitives/lambda.hpp>
#include <phxpr/primitives/placeholder.hpp>

namespace phxpr {

struct PHXPR_EXPORT evaluator;

typedef prana::parse_tree<prana::tag::sexpr> sexpr_parse_tree;

inline utree evaluate (sexpr_parse_tree const& pt);

utree evaluate (sexpr_parse_tree const& pt, evaluator& ev) PHXPR_EXPORT;

inline utree evaluate (utree const& ut);

utree evaluate (utree const& ut, evaluator& ev) PHXPR_EXPORT;

struct PHXPR_EXPORT evaluator: boost::noncopyable {
  // {{{ types
  typedef utree result_type;

  typedef boost::spirit::utf8_symbol_range_type symbol_type;
  typedef boost::iterator_range<utree::const_iterator> range_type;

  typedef compile_environment<utree, utree> variables_type;
  typedef sheol::adt::dynamic_array<signature> gpt_type;
  // }}}

  boost::shared_ptr<variables_type> variables;
  boost::shared_ptr<gpt_type> global_procedure_table;
  const displacement frame;
  signature sig;

  evaluator (void):
    variables(boost::make_shared<variables_type>()),
    global_procedure_table(boost::make_shared<gpt_type>()),
    frame(0), sig(0, arity_type::fixed, evaluation_strategy::call_by_value,
                  function_type::module, 0) 
  { }

  evaluator (boost::shared_ptr<variables_type> const& parent_,
             boost::shared_ptr<gpt_type> const& gpt,
             displacement frame_, signature const& sig_):
    variables(boost::make_shared<variables_type>(parent_)),
    global_procedure_table(gpt), frame(frame_), sig(sig_)
  { BOOST_ASSERT(parent_); }

  // REVIEW: Can we instead return utree(boost::ref(val)) safely?
  template <typename T>
  result_type operator() (T const& val)
  { return utree(val); }

  result_type operator() (range_type const& range)
  { return execute(range); }
  
  result_type operator() (symbol_type const& str)
  { return make_variable_reference(str); }

  template <typename F>
  void define_intrinsic (std::string const& name, F const& f) {
    boost::shared_ptr<function_body> body
      = boost::make_shared<function_body>(f, global_procedure_table);

    // create a new lambda expression
    lambda l(body, global_procedure_table, f.sig);

    // evaluate the lambda expression, returning a procedure
    boost::shared_ptr<runtime_environment> new_env
      = boost::make_shared<runtime_environment>(); 
    utree proc = new_env->invoke(l);

    boost::shared_ptr<utree> p = variables->define
      (utree(spirit::utf8_symbol_type(name)), proc); 

    global_procedure_table->push_back(f.sig);
    p->tag(global_procedure_table->size() - 1);
  }

  void define_global (std::string const& name, utree const& val)
  { variables->define(utree(spirit::utf8_symbol_type(name)), val); }

 private:
  result_type execute (range_type const& range);

  // FIXME: This requires a string copy because utree can't hold symbol ranges. 
  result_type make_variable_reference (symbol_type const& str) {
    boost::shared_ptr<utree> p = variables->lookup(utree(str));

    if (!p)
      BOOST_THROW_EXCEPTION(identifier_not_found(utree(str)));

    return *p; 
  }

  result_type
  make_lambda_expression (utree const& formals, range_type const& body);

  void make_placeholders (utree const& formals, signature const& sig);

  result_type
  make_if_thunk (utree const& test, utree const& then);

  result_type
  make_thunk (utree const& elements, signature const& sig);

  result_type
  make_if_else_thunk (utree const& test, utree const& then, utree const& else_);

  result_type
  make_internal_variable_thunk (utree const& value, displacement n,
                                displacement frame);
  
  result_type
  make_variable_reference_thunk (utree const& datum);

  result_type
  make_module_level_variable (utree const& identifier, utree const& value) {
    boost::shared_ptr<utree> p(variables->declare(identifier));
    *p = evaluate(value, *this);
    // return value of a definition is unspecified (aka invalid utree)
    return utree();
  }

  result_type
  make_internal_variable (utree const& identifier, utree const& value) {
    // IMPLEMENT: Don't use placeholder, use some sort of thunk that will
    // set value when invoked.
    using boost::fusion::at_c;
    const displacement n = at_c<4>(sig)++ + at_c<0>(sig);
    variables->define(identifier, utree(new placeholder(n, frame))); 
    return make_internal_variable_thunk(value, n, frame);
  }
};

inline utree evaluate (sexpr_parse_tree const& pt) {
  evaluator ev;
  return evaluate(pt, ev);
}

inline utree evaluate (utree const& ut) {
  evaluator ev;
  return evaluate(ut, ev);
}

} // phxpr

#endif // PHXPR_8A7BB292_8252_42D4_AEC4_F7F827B95E68

