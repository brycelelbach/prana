////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_BEBE64C6_AF30_47CF_AF44_72F5B995B84B)
#define PHXPR_BEBE64C6_AF30_47CF_AF44_72F5B995B84B

#include <phxpr/config.hpp>

#include <string>

#include <boost/assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/unordered_map.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/support_utree.hpp>

#include <phxpr/signature.hpp>

namespace phxpr {

template <typename T, typename Key = boost::iterator_range<char const*> >
struct environment {
  // {{{ types
  typedef boost::unordered_map<Key, boost::shared_ptr<T> > map_type;

  typedef typename map_type::mapped_type mapped_type;
  typedef typename map_type::key_type key_type;
  // }}}

  environment (void): parent(0), definitions(), next_local_variable(0) { }

  environment (environment* parent_):
    parent(parent_), definitions(), next_local_variable(0) { }

  mapped_type lookup (key_type const& name) {
    // {{{
    typename map_type::iterator it = definitions.find(name);

    // If we didn't find a definition and we have a parent, check the parent
    // for a definition.
    if (it == definitions.end()) {
      if (parent)
        return (*parent)[name];

      return mapped_type();
    }

    return it->second; 
  } // }}}

  mapped_type define (key_type const& name, T const& val) {
    // TODO: replace with exception (multiple definition)
    BOOST_ASSERT(!definitions.count(name));
    return (definitions[name] = boost::make_shared<T>(val));
  }

  bool defined (key_type const& name) const {
    // {{{
    if (!definitions.count(name)) {
      if (parent)
        return parent->defined(name);

      return false;
    }

    return true; 
  } // }}}
  
  bool locally_defined (key_type const& name) const {
    return definitions.count(name); 
  } 

  displacement next_local_variable;  

 private:
  environment* parent;
  map_type definitions;
};

} // phxpr

#endif // PHXPR_BEBE64C6_AF30_47CF_AF44_72F5B995B84B

