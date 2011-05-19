////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_8A7BB292_8252_42D4_AEC4_F7F827B95E68)
#define PHXPR_8A7BB292_8252_42D4_AEC4_F7F827B95E68

#include <phxpr/config.hpp>
#include <phxpr/environment.hpp>
#include <phxpr/signature.hpp>

#include <sheol/adt/dynamic_array.hpp>

#include <prana/utree/hash.hpp>
#include <prana/parse/parse_tree.hpp> 
#include <prana/parse/grammar/sexpr.hpp>

namespace phxpr {

struct evaluator {
  // {{{ types
  typedef utree result_type;

  typedef boost::spirit::utf8_symbol_range_type symbol_type;
  typedef boost::iterator_range<utree::const_iterator> range_type;

  typedef environment<utree, utree> variables_type;
  typedef sheol::adt::dynamic_array<signature> gpt_type;
  // }}}

  boost::shared_ptr<variables_type> variables;
  boost::shared_ptr<gpt_type> global_procedure_table;
  displacement num_local_variables;
  const displacement frame;

  evaluator (void):
    variables(boost::make_shared<variables_type>()),
    global_procedure_table(boost::make_shared<gpt_type>()),
    num_local_variables(0), frame(0) { }

  evaluator (boost::shared_ptr<variables_type> const& parent_,
             boost::shared_ptr<gpt_type> const& gpt,
             displacement frame_):
    variables(boost::make_shared<variables_type>(parent_)),
    global_procedure_table(gpt), num_local_variables(0), frame(frame_)
    { BOOST_ASSERT(parent_); }

  // REVIEW: Can we instead return utree(boost::ref(val)) safely?
  template <typename T>
  result_type operator() (T const& val)
  { return utree(val); }

  // FIXME: This requires a string copy because utree can't hold symbol ranges. 
  result_type operator() (symbol_type const& str);

  result_type operator() (range_type const& range);
};

typedef prana::parse_tree<prana::tag::sexpr> sexpr_parse_tree;

utree evaluate (sexpr_parse_tree const& pt);

utree evaluate (utree const& ut);

utree evaluate (utree const& ut, evaluator& ev);

} // phxpr

#endif // PHXPR_8A7BB292_8252_42D4_AEC4_F7F827B95E68

