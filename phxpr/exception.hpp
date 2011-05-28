////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_2F9A7918_D1BE_49F1_BA9C_47C2CB1F3BD1)
#define PHXPR_2F9A7918_D1BE_49F1_BA9C_47C2CB1F3BD1

#include <phxpr/config.hpp>

#include <boost/throw_exception.hpp>

#include <prana/include/utree.hpp>

#include <phxpr/signature.hpp>

// TODO: Pass runtime_environments instead of utrees holding
// runtime_environments to ease debugging with GDB.

namespace phxpr {

struct runtime_environment;

struct exception: virtual std::exception {
  virtual ~exception (void) throw() { }
};

struct invalid_arity: virtual exception {
  displacement expected;
  displacement got;
  arity_type::info type;

  invalid_arity (displacement expected_, displacement got_,
                 arity_type::info type_):
    expected(expected_), got(got_), type(type_) { }

  virtual ~invalid_arity (void) throw() { }
};

struct invalid_placeholder: virtual exception {
  displacement expected_n;
  displacement expected_frame;
  utree environment;
  arity_type::info type;

  invalid_placeholder (displacement expected_n_, displacement expected_frame_,
                       utree const& environment_, arity_type::info type_):
    expected_n(expected_n_), expected_frame(expected_frame_),
    environment(environment_), type(type_) { }

  virtual ~invalid_placeholder (void) throw() { }
};

struct invalid_local_variable: virtual exception {
  displacement expected;
  utree environment;

  invalid_local_variable (displacement expected_, utree const& environment_):
    expected(expected_), environment(environment_) { }

  virtual ~invalid_local_variable (void) throw() { }
};

struct invalid_operator_expression: virtual exception {
  utree expression;

  invalid_operator_expression (utree const& expression_):
    expression(expression_) { }
  
  virtual ~invalid_operator_expression (void) throw() { }
};

struct procedure_call_or_macro_use_expected: virtual exception {
  utree expression;

  procedure_call_or_macro_use_expected (utree const& expression_):
    expression(expression_) { }
  
  virtual ~procedure_call_or_macro_use_expected (void) throw() { }
};

struct identifier_not_found: virtual exception {
  utree symbol;

  identifier_not_found (utree const& symbol_): symbol(symbol_) { }
  
  virtual ~identifier_not_found (void) throw() { }
};

struct expected_formals_list: virtual exception {
  utree got;

  expected_formals_list (utree const& got_): got(got_) { }
  
  virtual ~expected_formals_list (void) throw() { }
};

struct expected_body: virtual exception {
  utree got;

  expected_body (utree const& got_): got(got_) { }
  
  virtual ~expected_body (void) throw() { }
};

struct expected_identifier: virtual exception {
  utree got;

  expected_identifier (utree const& got_): got(got_) { }
  
  virtual ~expected_identifier (void) throw() { }
};

struct expected_variable_argument: virtual exception {
  displacement n;
  boost::shared_ptr<runtime_environment> environment;
 
  expected_variable_argument (displacement n_, 
                              boost::shared_ptr<runtime_environment> const& e_):
    n(n_), environment(e_) { }
  
  virtual ~expected_variable_argument (void) throw() { }
};

struct unsupported_arity_type: virtual exception {
  arity_type::info type;

  unsupported_arity_type (arity_type::info type_): type(type_) { }
  
  virtual ~unsupported_arity_type (void) throw() { }
};

template <typename Key>
struct multiple_definitions: virtual exception {
  Key identifier;

  multiple_definitions (Key const& identifier_): identifier(identifier_) { }
  
  virtual ~multiple_definitions (void) throw() { }
};

} // phxpr

#endif // PHXPR_2F9A7918_D1BE_49F1_BA9C_47C2CB1F3BD1

