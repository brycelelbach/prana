/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_MACRO_PATTERN_MATCHER_HPP)
#define BOOST_SPIRIT_PRANA_VM_MACRO_PATTERN_MATCHER_HPP

namespace boost {
namespace spirit {
namespace prana {

struct pattern_matcher: std::binary_function<utree, utree, bool> {
  typedef bool result_type;

  typedef utree::const_range list;
  typedef utf8_symbol_range_type symbol;

  macro_literals* lits; 

  pattern_matcher (macro_literals* lits_ = 0);
 
  result_type operator() (utree const& match, utree const& patt) const;

  result_type operator() (list const& match, list const& patt) const;

  template<class Match, class Patt>
  result_type operator() (Match const& match, Patt const& patt) const;

  struct predicate {
    typedef bool result_type;
  
    typedef utree::const_range list;
    typedef utf8_symbol_range_type symbol;

    macro_literals::const_iterator lits; 

    predicate (macro_literals::const_iterator lits_);

    result_type operator() (list const& match, list const& patt) const;
    
    result_type operator() (list const& match, symbol const& patt) const;
    
    result_type operator() (symbol const& match, symbol const& patt) const;

    template<class Match>
    result_type operator() (Match const& match, symbol const& patt) const;

    template<class Match, class Patt>
    result_type operator() (Match const& match, Patt const& patt) const;
  };
};

pattern_matcher::pattern_matcher (macro_literals* lits_): lits(lits_) { }
 
pattern_matcher::result_type
pattern_matcher::operator() (utree const& match, utree const& patt) const {
  if (!lits)
    throw compilation_error();

  return utree::visit(match, patt, *this);
}

pattern_matcher::result_type
pattern_matcher::operator() (list const& match, list const& patt) const {
  if (match.empty() || patt.empty())
    throw expected_patt(utree(match, shallow), utree(patt, shallow));

  std::string match_name = get_symbol(*match.begin()),
              patt_name = get_symbol(*patt.begin());

  if (match_name != patt_name)
    return false;

  if (!lits->count(patt_name))
    throw macro_literals_not_found(patt_name);

  return utree::visit(
    spirit::rest(match), spirit::rest(patt),
    predicate(lits->find(patt_name))
  );
}

template<class Match, class Patt>
pattern_matcher::result_type
pattern_matcher::operator() (Match const& match, Patt const& patt) const {
  throw expected_pattern(utree(match), utree(patt));
  return false;
}

pattern_matcher::predicate::predicate (macro_literals::const_iterator lits_):
  lits(lits_) { }

pattern_matcher::predicate::result_type
pattern_matcher::predicate::operator() (list const& match,
                                        list const& patt) const
{
}

pattern_matcher::predicate::result_type
pattern_matcher::predicate::operator() (list const& match,
                                        symbol const& patt) const
{

}

pattern_matcher::predicate::result_type
pattern_matcher::predicate::operator() (symbol const& match,
                                        symbol const& patt) const
{
}

template<class Match>
pattern_matcher::predicate::result_type
pattern_matcher::predicate::operator() (Match const& match,
                                        symbol const& patt) const
{
}

template<class Match, class Patt>
pattern_matcher::predicate::result_type
pattern_matcher::predicate::operator() (Match const& match,
                                        Patt const& patt) const
{
  throw expected_pattern(utree(match), utree(patt));
  return false;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_MACRO_PATTERN_MATCHER_HPP

