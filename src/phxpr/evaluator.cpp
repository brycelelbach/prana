////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <prana/dispatch/visit_ref.hpp>

#include <phxpr/evaluator.hpp>

namespace phxpr {

evaluator::result_type
evaluator::operator() (evaluator::symbol_type const& str) {
  // IMPLEMENT
  return result_type();
}

evaluator::result_type
evaluator::operator() (evaluator::range_type const& str) {
  // IMPLEMENT
  return result_type();
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

