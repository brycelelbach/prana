/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_MACRO_FWD_HPP)
#define BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_MACRO_FWD_HPP

#include <functional>

#include <boost/unordered_set.hpp>
#include <boost/function.hpp>

#include <boost/spirit/home/prana/support/utree_hash.hpp>
#include <boost/spirit/home/prana/vm/get_symbol.hpp>
#include <boost/spirit/home/prana/vm/macro/pattern_matcher.hpp>
#include <boost/spirit/home/prana/vm/core/function.hpp>
#include <boost/spirit/home/prana/vm/environment/basic.hpp>

namespace boost {
namespace spirit {
namespace prana {

typedef boost::function<utree(scope const&)> compiled_macro;

typedef boost::unordered_map<
  std::string, boost::unordered_set<std::string>
> macro_literals;

struct macro_environment:
  basic_environment<
    macro_environment, 
    compiled_macro, 
    compiled_macro*,
    utree,
    boost::hash<utree>,
    pattern_matcher
  >
{
  macro_literals lits;

  macro_environment (macro_environment* parent = 0);

  void declare_literal (std::string const& name, std::string const& literal);
 
  template<typename Function>
  void define (key_type const& name, Function const& f);

  result_type find (key_type const& name);

  utree invoke (key_type const& name);

  result_type sentinel (void) const;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_MACRO_FWD_HPP

