/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_MACRO_HPP)
#define BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_MACRO_HPP

#include <functional>
#include <set>

#include <boost/foreach.hpp>
#include <boost/function.hpp>

#include <boost/fusion/include/at_c.hpp>

#include <boost/spirit/home/prana/support/utree_rest.hpp>
#include <boost/spirit/home/prana/vm/get_symbol.hpp>
#include <boost/spirit/home/prana/vm/core/function.hpp>
#include <boost/spirit/home/prana/vm/environment/basic.hpp>

namespace boost {
namespace spirit {
namespace prana {

typedef boost::function<utree(scope const&)> compiled_macro;

typedef std::map<std::string, std::set<std::string> > macro_literals;

struct pattern: std::binary_function<utree, utree, bool> {
  typedef bool result_type;

  macro_literals* lits; 

  pattern (macro_literals* lits_ = 0): lits(lits_) { }
 
  bool operator() (utree const& x, utree const& y) const {
    if (!lits)
      throw compilation_error();

    return utree::visit(x, y, *this);
  }

  bool operator() (utree::const_range const& x,
                   utree::const_range const& y) const
  {
    if (x.empty() || y.empty())
      throw expected_pattern(utree(x, shallow), utree(y, shallow));

    std::string namex = get_symbol(*x.begin()),
                namey = get_symbol(*y.begin());

    if (namex != namey)
      return false;

    if (!lits->count(namex))
      throw macro_literals_not_found(namex);

    return match(lits->find(namex), spirit::rest(x), spirit::rest(y));
  }

  template<class X, class Y>
  bool operator() (X const& x, Y const& y) const {
    throw expected_pattern(utree(x), utree(y));
    return false;
  }

  bool match (macro_literals::const_iterator it, utree::const_range const& x,
              utree::const_range const& y) const;
};

bool pattern::match (macro_literals::const_iterator it,
                     utree::const_range const& x,
                     utree::const_range const& y) const
{
  typedef utree::const_range range_type;

  static utree_type::info const list_ = utree_type::list_type;
  static utree_type::info const range_ = utree_type::range_type;
  static utree_type::info const symbol_ = utree_type::symbol_type;

  if (x.empty() && y.empty())
    return true;

  else if (x.empty() && !y.empty())
    return false;

  else if (y.empty() && (get_symbol(x.back()) == "..."))
    return true;

  utree xx = *x.begin(),
        yy = *y.begin(),
        restx = spirit::rest(x),
        resty = spirit::rest(y); 

  // match the pattern of the nested list
  if (((xx.which() == list_) || (xx.which() == range_)) &&
      ((yy.which() == list_) || (yy.which() == range_)) &&
      !match(it, xx.get<range_type>(), yy.get<range_type>()))
    return false;

  else if (xx.which() == symbol_) { 
    // match the symbol against the literal set
    if (it->second.count(get_symbol(xx))) {
      if (yy.which() != symbol_)
        return false;

      if (get_symbol(yy) != get_symbol(xx))
        return false;
    }
  }
  
  else 
    throw expected_pattern(xx);

  // handle variable arguments
  if (!restx.empty() && (get_symbol(*restx.begin()) == "...")) {
    if (!spirit::rest(restx).empty())
      throw unexpected_identifier();

    return match(it, x, resty); 
  }
    
  return match(it, restx, resty);  
}

struct macro_environment:
  basic_environment<
    macro_environment, 
    compiled_macro, 
    compiled_macro*,
    utree,
    pattern
  >
{
  macro_literals lits;

  macro_environment (macro_environment* parent = 0):
    basic_environment(parent, pattern(&lits)), lits() { }

  void declare_literal (std::string const& name, std::string const& literal) {
    if (!lits.count(name))
      lits[name] = std::set<std::string>();

    lits[name].insert(literal);
  } 
 
  template<typename Function>
  void define (key_type const& name, Function const& f) {
    std::string str = get_symbol(*name.begin());

    if (!lits.count(str))
      lits[str] = std::set<std::string>();

    if (definitions.find(name) != definitions.end())
      throw duplicate_identifier();

    definitions[name] = compiled_macro(f);
  }

  result_type find (key_type const& name) {
    iterator i = definitions.find(name);

    if (i != definitions.end())
      return &i->second;
    else if (outer)
      return (*outer)(name);

    return sentinel();
  }

  result_type sentinel (void) const {
    return 0; 
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_MACRO_HPP

