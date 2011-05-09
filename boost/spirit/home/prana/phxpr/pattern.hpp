////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BOOST_SPIRIT_PRANA_21CAE933_28F5_486B_A85D_6F366103657B)
#define BOOST_SPIRIT_PRANA_21CAE933_28F5_486B_A85D_6F366103657B

#include <set>
#include <map>
#include <algorithm>

#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/next_prior.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct matcher {
  typedef std::set<utree> literals_type;
  typedef std::map<utree, utree> bindings_type;

 private:
  literals_type const& _literals;
  utree const& _body;
  mutable boost::shared_ptr<bindings_type> _bindings;

 public:
  typedef bool result_type;

  matcher (std::set<utree> const& literals_, utree const& body_):
    _literals(literals_), _body(body_), _bindings(new bindings_type()) { }

  matcher (matcher const& other):
    _literals(other._literals), _body(other._body),
    _bindings(other._bindings) { } 

  literals_type const& literals (void) const {
    return _literals;
  } 

  bindings_type const& bindings (void) const {
    BOOST_ASSERT(_bindings);
    return *_bindings;
  }

 private:
  void apply (utree const& body, utree& ut) const;
  
  boost::shared_ptr<utree> apply (utree const& body) const;
 
  void bind (utree& binding, utree const& rhs) const;

 public:
  boost::shared_ptr<utree> expand (void) const {
    return apply(_body);
  }

  // the lhs is always the pattern element and the rhs is always corresponding
  // element in the use being invoked.

  template<class T0, class T1>
  result_type operator() (T0 const&, T1 const&) const {
    // TODO: replace with exception (this should never really be reached,
    // though)
    BOOST_ASSERT(false);
  }

  template<class T>
  typename disable_if<is_same<function_base, T>, result_type>::type 
  operator() (utf8_symbol_range_type const& lhs, T const& rhs) const {
    utree ut(lhs);

    if (_literals.count(ut))
      return false;
    else {
      if (!_bindings->count(ut))
        (*_bindings)[ut] = rhs;
      else
        bind((*_bindings)[ut], rhs);
      return true;
    }
  }
  
  template<class T>
  typename disable_if<is_same<function_base, T>, result_type>::type 
  operator() (utree::const_range const& lhs, T const& rhs) const {
    return utree::visit(utree(lhs), utree(rhs), *this);
  }
  
  result_type operator() (utf8_symbol_range_type const& lhs,
                          utf8_symbol_range_type const& rhs) const;
  
  result_type operator() (utree::const_range const& lhs,
                          utree::const_range const& rhs) const;
}; 

struct pattern {
  typedef std::set<utree> literals_type;

 private:
  literals_type const& _literals;
  utree _elements;
  utree _body;

 public:
  typedef boost::shared_ptr<utree> result_type;

  pattern (literals_type const& literals_, utree const& elements_,
           utree const& body_):
    _literals(literals_), _elements(elements_), _body(body_)
  {
    // TODO: add checks here to ensure that elements and literals only contain
    // symbols
  }

  literals_type const& literals (void) const {
    return _literals;
  }

  utree const& elements (void) const {
    return _elements;
  }

  utree const& body (void) const {
    return _body;
  }

  boost::shared_ptr<matcher> match (utree const& use) const;
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_21CAE933_28F5_486B_A85D_6F366103657B

