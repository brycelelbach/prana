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
#include <boost/range/iterator_range.hpp>
#include <boost/unordered_map.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/support_utree.hpp>

#include <sheol/adt/dynamic_array.hpp>

#include <phxpr/gc/shared_ptr.hpp>
#include <phxpr/gc/enable_shared_from_this.hpp>
#include <phxpr/gc/make_shared.hpp>
#include <phxpr/exception.hpp>

namespace phxpr {

template <typename Key, typename T>
struct compile_environment {
  // {{{ types
  typedef boost::unordered_map<Key, phxpr::shared_ptr<T> > map_type;

  typedef typename map_type::mapped_type mapped_type;
  typedef typename map_type::key_type key_type;
  // }}}

  compile_environment (void): parent(), definitions() { }

  compile_environment (phxpr::shared_ptr<compile_environment> const& parent_):
    parent(parent_), definitions() { }

  mapped_type lookup (key_type const& name) {
    // {{{
    typename map_type::iterator it = definitions.find(name);

    // If we didn't find a definition and we have a parent, check the parent
    // for a definition.
    if (it == definitions.end()) {
      if (parent)
        return parent->lookup(name);

      return mapped_type();
    }

    return it->second; 
  } // }}}
  
  mapped_type& declare (key_type const& name) {
    if (definitions.count(name))
    { BOOST_THROW_EXCEPTION(multiple_definitions<Key>(name)); }
    return (definitions[name] = phxpr::shared_ptr<T>());
  }

  mapped_type& define (key_type const& name, T const& val) {
    if (definitions.count(name))
    { BOOST_THROW_EXCEPTION(multiple_definitions<Key>(name)); }
    return (definitions[name] = phxpr::make_shared<T>(val));
  }

  mapped_type& redefine (key_type const& name, T const& val) {
    return (definitions[name] = phxpr::make_shared<T>(val));
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

 private:
  phxpr::shared_ptr<compile_environment> parent;
  map_type definitions;
};
    
struct runtime_environment:
  boost::iterator_range<sheol::adt::dynamic_array<utree>::iterator>,
  phxpr::enable_shared_from_this<runtime_environment>
{
 public:
  typedef sheol::adt::dynamic_array<utree> upvalue_type;
  typedef boost::iterator_range<upvalue_type::iterator> base_type;
//  typedef boost::iterator_range<upvalue_type>::iterator iterator;

  runtime_environment (void): depth(0), parent(), upval() { }

  runtime_environment (phxpr::shared_ptr<runtime_environment> const& parent_):
    depth(parent_ ? (parent_->depth + 1) : 0), parent(parent_), upval() { }
  
  runtime_environment (phxpr::shared_ptr<upvalue_type> const& upval_):
    base_type(upval_->begin(), upval_->end()), depth(0),
    parent(), upval(upval_) { }
  
  runtime_environment (phxpr::shared_ptr<upvalue_type> const& upval_,
                       phxpr::shared_ptr<runtime_environment> const& parent_):
    base_type(upval_->begin(), upval_->end()),
    depth(parent_ ? (parent_->depth + 1) : 0), parent(parent_),
    upval(upval_) { }
  
  runtime_environment (runtime_environment const& other):
    base_type(other.begin(), other.end()),
    depth(other.depth), parent(other.parent), upval(other.upval) { }

/*
  ~runtime_environment (void) {
    if (upval.unique())
      for (iterator it = begin(), end_ = end(); it != end_; ++it) 
      { it->clear(); }
  }
*/

  phxpr::shared_ptr<runtime_environment> const& outer (void) const
  { return parent; }
  
  runtime_environment::size_type level (void) const 
  { return depth; }

  phxpr::shared_ptr<upvalue_type> const& get() const
  { return upval; }

  phxpr::shared_ptr<runtime_environment> checkout() const
  { return const_cast<runtime_environment*>(this)->shared_from_this(); }

  template <typename F>
  utree invoke (F const& f) const {
    utree ut(spirit::any_ptr(checkout().get()));
    return f.eval(ut);
  }
  
  template <typename F>
  utree invoke (phxpr::shared_ptr<F> const& f) const {
    utree ut(spirit::any_ptr(checkout().get()));
    return f->eval(ut);
  }

 private:
  const runtime_environment::size_type depth;
  phxpr::shared_ptr<runtime_environment> parent;
  phxpr::shared_ptr<upvalue_type> upval;
};

} // phxpr

#endif // PHXPR_BEBE64C6_AF30_47CF_AF44_72F5B995B84B

