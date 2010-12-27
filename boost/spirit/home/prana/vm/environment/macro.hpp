/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_MACRO_HPP)
#define BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_MACRO_HPP

#include <boost/spirit/home/prana/support/utree_rest.hpp>
#include <boost/spirit/home/prana/vm/macro.hpp>
#include <boost/spirit/home/prana/vm/environment/macro_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

macro_environment::macro_environment (macro_environment* parent):
  basic_environment(parent, boost::hash<utree>(), pattern_matcher(&lits)),
  lits() { }

void macro_environment::declare_literal (std::string const& name,
                                         std::string const& literal)
{
  if (!lits.count(name))
    lits[name] = boost::unordered_set<std::string>();

  lits[name].insert(literal);
} 

template<typename Function>
void macro_environment::define (key_type const& name, Function const& f) {
  std::string str = get_symbol(*name.begin());

  if (!lits.count(str))
    lits[str] = boost::unordered_set<std::string>();

  if (definitions.find(name) != definitions.end())
    throw duplicate_identifier();

  definitions[name] = compiled_macro(f);
}

macro_environment::result_type macro_environment::find (key_type const& name) {
  iterator i = definitions.find(name);

  if (i != definitions.end())
    return &i->second;
  else if (outer)
    return (*outer)(name);

  return sentinel();
}

macro_environment::utree macro_environment::invoke (key_type const& name) {
  iterator i = definitions.find(name);

  if (i == definitions.end()) {
    if (outer)
      return outer->invoke(name);
    else
      throw expected_match(name);
  }

  std::string str = get_symbol(*name.begin());

  macro_literals::const_iterator l = lits.find(str);

  if (l == lits.end())
    throw compilation_error();

  return invoker(&l, &i->second)(name, i->first);
}

macro_environment::result_type macro_environment::sentinel (void) const {
  return 0; 
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_MACRO_HPP

