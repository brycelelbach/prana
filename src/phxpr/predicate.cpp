////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <phxpr/predicate.hpp>

namespace phxpr {

struct boolean_visitor {
  typedef bool result_type;

  result_type operator() (utree::invalid_type) const
  { return false; }

  result_type operator() (utree::nil_type) const 
  { return false; }

  result_type operator() (int i) const 
  { return i; }

  result_type operator() (double d) const 
  { return d; }

  result_type operator() (bool b) const 
  { return b; }

  result_type operator() (spirit::binary_range_type const& str) const 
  { return !str.empty(); }

  result_type operator() (spirit::utf8_string_range_type const& str) const
  { return !str.empty(); }

  result_type operator() (spirit::utf8_symbol_range_type const& str) const
  { return !str.empty(); }

  template <typename Iterator>
  result_type operator() (boost::iterator_range<Iterator> const& range) const
  { return !range.empty(); }

  result_type operator() (spirit::any_ptr const& p) const
  { return true; } 

  result_type operator() (spirit::function_base const& pf) const
  { return true; }
};

bool predicate (utree const& pred) {
  return utree::visit(pred, boolean_visitor());
}

} // phxpr

