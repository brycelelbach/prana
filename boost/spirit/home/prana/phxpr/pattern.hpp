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
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct is_variadic_predicate {
  typedef bool result_type;

  template<class T>
  result_type operator() (T const&) const {
    std::cout << "is_variadic - T" << std::endl;
    return false;
  }

  result_type operator() (utree::const_range const& ut) const {
    std::cout << "is_variadic - range" << std::endl;
    return utree::visit(ut.back(), is_variadic_predicate());
  }

  result_type operator() (utf8_symbol_range_type const& ut) const {
    std::cout << "is_variadic - symbol" << std::endl;
    return ut == std::string("...");
  }
}; 

template<class T>
inline bool is_variadic (T const& val) {
  return is_variadic_predicate()(val);
} 

inline bool is_variadic (utree const& val) {
  return utree::visit(val, is_variadic_predicate());
}

struct matcher {
  typedef std::set<utree> literals_type;
  typedef std::map<utree, utree> bindings_type;

 private:
  literals_type const& _literals;
  mutable boost::shared_ptr<bindings_type> _bindings;

 public:
  typedef bool result_type;

  matcher (std::set<utree> const& literals_):
    _literals(literals_), _bindings(new bindings_type()) { }

  matcher (matcher const& other):
    _literals(other._literals), _bindings(other._bindings) { } 

  literals_type const& literals (void) const {
    return _literals;
  } 

  bindings_type const& bindings (void) const {
    BOOST_ASSERT(_bindings);
    return *_bindings;
  }

  boost::shared_ptr<utree> apply (utree const& body) {
    // TODO: implement
    return boost::shared_ptr<utree>();
  }

  // the lhs is always the pattern element and the rhs is always corresponding
  // element in the use being invoked.

  template<class T0, class T1>
  result_type operator() (T0 const&, T1 const&) const {
    // TODO: replace with exception (this should never really be reached,
    // though
    BOOST_ASSERT(false);
  }

  template<class T>
  typename disable_if<is_same<function_base, T>, result_type>::type 
  operator() (utf8_symbol_range_type const& lhs, T const& rhs) const {
    std::cout << "symbol-T" << std::endl; 
    utree ut(lhs);

    if (_literals.count(ut))
      return false;
    else {
      if (!_bindings->count(ut))
        (*_bindings)[ut] = utree::list_type();
      (*_bindings)[ut].push_back(rhs);
      return true;
    }
  }
  
  template<class T>
  typename disable_if<is_same<function_base, T>, result_type>::type 
  operator() (utree::const_range const& lhs, T const& rhs) const {
    std::cout << "range-T" << std::endl; 
    switch (traits::size(lhs)) {
      case 0: {
        // TODO: replace with exception
        BOOST_ASSERT(false); 
        return false;
      }

      case 1: {
        return utree::visit(lhs.front(), utree(rhs), *this);
      }
      
      case 2: {
        if (is_variadic(lhs))
          return utree::visit(lhs.front(), utree(rhs), *this);
      }
 
      // if the lhs has more than two elements, it can't possibly match.
      default:
        return false;
    }
  }
  
  result_type operator() (utf8_symbol_range_type const& lhs,
                          utf8_symbol_range_type const& rhs) const
  {
    std::cout << "symbol-symbol" << std::endl; 
    utree ut(lhs);
    bindings_type::iterator it = _bindings->find(ut),
                            end = _bindings->end();

    if (it == end)
      return false;
    else if (_literals.count(ut))
      return !std::lexicographical_compare(lhs.begin(), lhs.end(),
                                           rhs.begin(), rhs.end());
    else {
      it->second.push_back(rhs);
      return true;
    }
  }
  
  result_type operator() (utf8_symbol_range_type const& lhs,
                          utree::const_range const& rhs) const 
  {
    std::cout << "symbol-range" << std::endl; 
    if (traits::size(rhs) == 1)
      return utree::visit(utree(lhs), rhs.front(), *this);
    else 
      // this can't possibly match
      return false;
  }
  
  result_type operator() (utree::const_range const& lhs,
                          utree::const_range const& rhs) const
  {
    std::cout << "range-range" << std::endl; 
    std::cout << "is_variadic: " << is_variadic(lhs) << std::endl;

    utree::const_range::const_iterator it = lhs.begin(),
                                            end = lhs.end();

    if (is_variadic(lhs)) {
      std::cout <<   "lhs size: " << traits::size(lhs)
                << "\nrhs size: " << traits::size(rhs) << std::endl;

      if (traits::size(rhs) < (traits::size(lhs) - 2))
        return false;
     
      // initialize the variable argument to a list, so that it's not invalid
      // if it's empty 
      utree::const_range::const_iterator vararg = boost::prior(end, 2);
      utree ut(*vararg);
      (*_bindings)[ut] = utree::list_type();
    }

    else if (traits::size(lhs) != traits::size(rhs))
      return false;

    BOOST_FOREACH(utree const& ut, rhs) {
      if (it == end)
        return false;

      std::cout <<   "lhs: " << *it
                << "\nrhs: " << ut << std::endl;

      if (!utree::visit(*it, ut, *this))
        return false;

      if ((boost::next(it) == end) || !is_variadic(*boost::next(it)))
        ++it;
    }

    return true;
  }
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

  result_type operator() (utree const& use) const {
    matcher m(_literals);
    
    if (utree::visit(_elements, use, m))
      return m.apply(_body);
    else
      return result_type();
  }

  boost::shared_ptr<matcher> match (utree const& use) const {
    boost::shared_ptr<matcher> m(new matcher(_literals));

    if (utree::visit(_elements, use, *m))
      return m;
    else
      return boost::shared_ptr<matcher>();
  }
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_21CAE933_28F5_486B_A85D_6F366103657B

