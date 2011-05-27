////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_D2483FE9_7EFE_4BD5_A179_CFE5B4102F91)
#define PHXPR_D2483FE9_7EFE_4BD5_A179_CFE5B4102F91

#include <phxpr/config.hpp>

#include <prana/utree/predicates.hpp>

#include <phxpr/primitives/unary.hpp>

namespace phxpr {

struct boolean_predicate: phxpr::unary<boolean_predicate> {
  utree eval (utree const& subject) const
  { return prana::recursive_which(subject) == utree_type::bool_type; }
};

struct symbol_predicate: phxpr::unary<symbol_predicate> {
  utree eval (utree const& subject) const
  { return prana::recursive_which(subject) == utree_type::symbol_type; }
};

struct procedure_predicate: phxpr::unary<procedure_predicate> {
  utree eval (utree const& subject) const
  { return prana::recursive_which(subject) == utree_type::function_type; }
};

struct list_predicate: phxpr::unary<list_predicate> {
  utree eval (utree const& subject) const
  { return prana::is_utree_container(subject); }
};

struct number_predicate: phxpr::unary<number_predicate> {
  utree eval (utree const& subject) const {
    return prana::recursive_which(subject) == utree_type::int_type
        || prana::recursive_which(subject) == utree_type::double_type;
  }
};

struct string_predicate: phxpr::unary<string_predicate> {
  utree eval (utree const& subject) const
  { return prana::is_utree_string(subject); }
};

struct nil_predicate: phxpr::unary<nil_predicate> {
  utree eval (utree const& subject) const
  { return prana::recursive_which(subject) == utree_type::nil_type; }
};

struct invalid_predicate: phxpr::unary<invalid_predicate> {
  utree eval (utree const& subject) const
  { return prana::recursive_which(subject) == utree_type::invalid_type; }
};

} // phxpr

#endif // PHXPR_D2483FE9_7EFE_4BD5_A179_CFE5B4102F91

