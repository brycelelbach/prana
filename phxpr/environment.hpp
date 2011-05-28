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
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/unordered_map.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/support_utree.hpp>

#include <phxpr/exception.hpp>

namespace phxpr {

template <typename T, typename Key = boost::iterator_range<char const*> >
struct compile_environment {
  // {{{ types
  typedef boost::unordered_map<Key, boost::shared_ptr<T> > map_type;

  typedef typename map_type::mapped_type mapped_type;
  typedef typename map_type::key_type key_type;
  // }}}

  compile_environment (void): parent(), definitions() { }

  compile_environment (boost::shared_ptr<compile_environment> const& parent_):
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
  
  mapped_type declare (key_type const& name) {
    if (definitions.count(name))
    { BOOST_THROW_EXCEPTION(multiple_definitions<Key>(name)); }
    return (definitions[name] = boost::make_shared<T>());
  }

  mapped_type define (key_type const& name, T const& val) {
    if (definitions.count(name))
    { BOOST_THROW_EXCEPTION(multiple_definitions<Key>(name)); }
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

 private:
  boost::shared_ptr<compile_environment> parent;
  map_type definitions;
};
    
struct runtime_environment:
  boost::iterator_range<utree*>,
  boost::enable_shared_from_this<runtime_environment>
{
 public:
  runtime_environment (void): depth(0), parent(), upval(0) { }

  runtime_environment (boost::shared_ptr<runtime_environment> const& parent_):
    depth(parent_ ? (parent_->depth + 1) : 0), parent(parent_), upval(0) { }
  
  runtime_environment (boost::shared_array<utree> const& upval_,
                       runtime_environment::size_type size_):
    boost::iterator_range<utree*>(upval_.get(), upval_.get() + size_), depth(0),
    parent(), upval(upval_) { }
  
  runtime_environment (boost::shared_array<utree> const& upval_,
                       runtime_environment::size_type size_,
                       boost::shared_ptr<runtime_environment> const& parent_):
    boost::iterator_range<utree*>(upval_.get(), upval_.get() + size_),
    depth(parent_ ? (parent_->depth + 1) : 0), parent(parent_),
    upval(upval_) { }
  
  runtime_environment (runtime_environment const& other):
    boost::iterator_range<utree*>(other.begin(), other.end()),
    depth(other.depth), parent(other.parent), upval(other.upval) { }

  boost::shared_ptr<runtime_environment> const& outer (void) const
  { return parent; }
  
  runtime_environment::size_type level (void) const 
  { return depth; }

  boost::shared_array<utree> const& get() const
  { return upval; }

  boost::shared_ptr<runtime_environment> checkout() const
  { return const_cast<runtime_environment*>(this)->shared_from_this(); }

  template <typename F>
  utree invoke (F const& f) const
  { return f.eval(utree(spirit::any_ptr(checkout().get()))); }
  
  template <typename F>
  utree invoke (boost::shared_ptr<F> const& f) const
  { return f->eval(utree(spirit::any_ptr(checkout().get()))); }

 private:
  runtime_environment::size_type depth;
  boost::shared_ptr<runtime_environment> parent;
  boost::shared_array<utree> upval;
};

} // phxpr

#endif // PHXPR_BEBE64C6_AF30_47CF_AF44_72F5B995B84B

