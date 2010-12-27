/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_MACRO_INVOKER_HPP)
#define BOOST_SPIRIT_PRANA_VM_MACRO_INVOKER_HPP

namespace boost {
namespace spirit {
namespace prana {

struct invoker {
  typedef utree result_type;
  
  typedef utree::const_range list;
  typedef utf8_symbol_range_type symbol;

  utree const& template_;
  utree macro;
  macro_literals::const_iterator lits; 
  boost::unordered_map<symbol, utree> bindings;

  invoker (utree const& temp, macro_literals::const_iterator lits_);
  
  result_type operator() (utree const& match, utree const& patt) const;
  
  struct binder { 
    typedef void result_type;
  
    typedef utree::const_range list;
    typedef utf8_symbol_range_type symbol;

    utree const* template_;
    utree* macro;
    macro_literals::const_iterator* lits; 
    boost::unordered_map<symbol, utree>* bindings;

    binder (utree* temp, utree* macro_, macro_literals::const_iterator* lits_,
            boost::unordered_map<symbol, utree>* bindings_);
    
    result_type operator() (list const& match, list const& patt) const;

    result_type operator() (list const& match, symbol const& patt) const;

    result_type operator() (symbol const& match, symbol const& patt) const;
    
    template<class Match>
    result_type operator() (Match const& match, symbol const& patt) const;

    template<class Match, class Patt>
    result_type operator() (Match const& match, Patt const& patt) const;
  };
};
  
invoker::invoker (utree const& temp, macro_literals::const_iterator* lits_)
  template_(temp), lits(lits_) { }
 
invoker::result_type
invoker::operator() (utree const& match, utree const& patt) const {
  utree::visit(match, patt, binder(lits, &bindings));
}

invoker::binder::binder (utree* temp, utree* macro_,
                         macro_literals::const_iterator* lits_,
                         boost::unordered_map<symbol, utree>* bindings_)
  template_(temp), macro(macro_), lits(lits_), bindings(bindings_) { }

invoker::binder::result_type
invoker::binder::operator() (utree const& match, utree const& patt) const {
}

invoker::binder::result_type
invoker::binder::operator() (list const& match, list const& patt) const {
}

invoker::binder::result_type
invoker::binder::operator() (list const& match, symbol const& patt) const {
}

invoker::binder::result_type
invoker::binder::operator() (symbol const& match, symbol const& patt) const {
}

template<class Match>
invoker::binder::result_type
invoker::binder::operator() (Match const& match, symbol const& patt) const {
  throw expected_match(utree(match), utree(patt));
}

template<class Match, class Patt>
invoker::binder::result_type
invoker::binder::operator() (Match const& match, Patt const& patt) const {
  throw expected_match(utree(match), utree(patt));
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_MACRO_INVOKER_HPP

