////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <phxpr/config.hpp>

#include <prana/dispatch/visit_ref.hpp>

#include <phxpr/evaluator.hpp>
  
namespace phxpr {

utree evaluate (sexpr_parse_tree const& pt, evaluator& ev) {
  return utree::visit(pt.ast(), prana::visit_ref(ev));
}

utree evaluate (utree const& ut, evaluator& ev) {
  return utree::visit(ut, prana::visit_ref(ev));
}

} // phxpr

