////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/foreach.hpp>

#include <boost/spirit/home/prana/phxpr/pattern.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct is_variadic_visitor {
  typedef bool result_type;

  template<class T>
  result_type operator() (T const&) const {
    return false;
  }

  result_type operator() (utree::const_range const& ut) const {
    return utree::visit(ut.back(), is_variadic_visitor());
  }

  result_type operator() (utf8_symbol_range_type const& ut) const {
    return ut == std::string("...");
  }
}; 

template<class T>
inline bool is_variadic (T const& val) {
  return is_variadic_visitor()(val);
} 

inline bool is_variadic (utree const& val) {
  return utree::visit(val, is_variadic_visitor());
}

struct initialize_to_list_visitor {
  typedef std::map<utree, utree> bindings_type;

 private:
  mutable boost::shared_ptr<bindings_type> _bindings;

 public:
  typedef bool result_type;

  initialize_to_list_visitor (boost::shared_ptr<bindings_type>& bindings_):
    _bindings(bindings_) { }

  initialize_to_list_visitor (initialize_to_list_visitor const& other):
    _bindings(other._bindings) { } 

  template<class T>
  result_type operator() (T const&) const {
    // TODO: replace with exception (this should never really be reached,
    // though)
    BOOST_ASSERT(false);
    return false;
  }

  result_type operator() (utree::const_range const& ut) const {
    BOOST_FOREACH(utree const& element, ut) {
      utree::visit(element, *this);
    }
    return true;
  }

  result_type operator() (utf8_symbol_range_type const& ut) const {
    utree name(ut);

    if (name != utree(utf8_symbol_type("...")))
      (*_bindings)[name] = utree::list_type();

    return true;
  }
}; 

template<class T, class Bindings>
inline bool initialize_to_list (T const& val, Bindings& bindings) {
  return initialize_to_list_visitor(bindings)(val);
} 

template<class Bindings>
inline bool initialize_to_list (utree const& val, Bindings& bindings) {
  return utree::visit(val, initialize_to_list_visitor(bindings));
}

void matcher::apply (utree const& body, utree& ut) const {
  switch (body.which()) {
    case utree_type::reference_type: {
      apply(body.deref(), ut);
      return;
    }
    
    case utree_type::invalid_type: {
      // TODO: make this an exception
      BOOST_ASSERT(false);
      return;
    }

    case utree_type::list_type:
    case utree_type::range_type: {
      // put an empty list in the parent utree
      ut.push_back(utree::list_type());
      // fill the new list
      BOOST_FOREACH(utree const& element, body) {
        apply(element, ut.back());
      }
      return;
    }

    case utree_type::symbol_type: {
      bindings_type::const_iterator it = _bindings->find(body),
                                    end = _bindings->end();
      
      // if we found a match, we replace the symbol with it, and then expand
      // the replacement.
      if (it != end) {
        apply(it->second, ut);
        return;
      }

      // if we didn't find a match, add the symbol as-is
      ut.push_back(body);
      return; 
    }

    default: {
      ut.push_back(body);
      return;
    }    
  }
}

boost::shared_ptr<utree> matcher::apply (utree const& body) const {
  switch (body.which()) {
    case utree_type::reference_type: {
      return apply(body.deref());
    }
    
    case utree_type::invalid_type: {
      // TODO: make this an exception
      BOOST_ASSERT(false);
      return boost::shared_ptr<utree>();
    }

    case utree_type::list_type:
    case utree_type::range_type: {
      // the replacement is a list, so we need to apply the bindings to the
      // replacement elements.
      boost::shared_ptr<utree> r(new utree());
      *r = utree::list_type();
      BOOST_FOREACH(utree const& element, body) {
        apply(element, *r);
      }
      return r;
    }

    case utree_type::symbol_type: {
      bindings_type::const_iterator it = _bindings->find(body),
                                    end = _bindings->end();
      
      // if we found a match, we replace the symbol with it, and then expand
      // the replacement.
      if (it != end)
        return apply(it->second);

      // if we didn't find a match, return the symbol as-is
      return boost::shared_ptr<utree>(new utree(body));
    }

    default: {
      return boost::shared_ptr<utree>(new utree(body));
    }    
  }
}

void matcher::bind (utree& binding, utree const& rhs) const {
  switch (binding.which()) {
    case utree_type::reference_type: {
      bind(binding.deref(), rhs);
      return;
    }
    
    case utree_type::invalid_type:
    case utree_type::list_type:
    case utree_type::range_type: {
      binding.push_back(rhs);
      return;
    }

    default: {
      utree ut = binding;
      binding = utree::list_type();
      ut.push_back(binding);
      ut.push_back(rhs);
      return;
    }    
  }
}

matcher::result_type
matcher::operator() (utf8_symbol_range_type const& lhs,
                     utf8_symbol_range_type const& rhs) const
{
  utree ut(lhs);
  bindings_type::iterator it = _bindings->find(ut),
                          end = _bindings->end();

  if (_literals.count(ut))
    return !std::lexicographical_compare(lhs.begin(), lhs.end(),
                                         rhs.begin(), rhs.end());
  else {
    if (!_bindings->count(ut))
      (*_bindings)[ut] = rhs;
    else
      bind((*_bindings)[ut], rhs);
    return true;
  }
}

matcher::result_type
matcher::operator() (utree::const_range const& lhs,
                     utree::const_range const& rhs) const
{
  utree::const_range::const_iterator it = lhs.begin(),
                                          end = lhs.end();

  if (is_variadic(lhs)) {
    if (traits::size(rhs) < (traits::size(lhs) - 2))
      return false;
   
    // initialize the variable argument to a list, so that it's not invalid
    // if it's empty
    utree::const_range::const_iterator vararg = boost::prior(end, 2);
    initialize_to_list(utree(*vararg), _bindings);
  }

  else if (traits::size(lhs) != traits::size(rhs))
    return false;

  BOOST_FOREACH(utree const& ut, rhs) {
    if (it == end)
      return false;

    if (!utree::visit(*it, ut, *this))
      return false;

    if ((boost::next(it) == end) || !is_variadic(*boost::next(it)))
      ++it;
  }

  return true;
}

boost::shared_ptr<matcher> pattern::match (utree const& use) const {
  boost::shared_ptr<matcher> m(new matcher(_literals, _body));

  if (utree::visit(_elements, use, *m))
    return m;
  else
    return boost::shared_ptr<matcher>();
}

} // phxpr
} // prana
} // spirit
} // boost

