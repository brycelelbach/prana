/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_DE441280_828F_4B7F_B79F_5EA7766C9625)
#define BSP_DE441280_828F_4B7F_B79F_5EA7766C9625

#include <string>

#include <boost/assert.hpp>
#include <boost/unordered_map.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/spirit/home/support/utree.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

template<class Value, class Key = std::string,
         class Hash = boost::hash<Key>, class Pred = std::equal_to<Key> >
struct environment {
  typedef Value mapped_type;
  typedef Key key_type;

  typedef boost::unordered_map<key_type, mapped_type, Hash, Pred> map_type;

  typedef typename map_type::iterator iterator;
  typedef typename map_type::const_iterator const_iterator;

  typedef typename map_type::size_type size_type;
  
  typedef typename map_type::value_type value_type;

  environment (environment* parent = 0, Hash const& hash = Hash(),
               Pred const& pred = Pred()):
    outer(parent), depth(parent ? parent->depth + 1 : 0),
    definitions(boost::unordered::detail::default_bucket_count, hash, pred) { }
 
  fusion::vector3<iterator, bool, scope::size_type>
  operator[] (key_type const& name) {
    iterator it = definitions.find(name),
             end = definitions.end();

    // If we didn't find the definition and we have an outer scope,
    // check the outer scope for the definition.
    if (it == end) {
      if (outer)
        return (*outer)[name];

      // If we found the definition or have no outer scope, return the
      // iterator and false.
      return fusion::vector3<iterator, bool, scope::size_type>
        (it, false, depth);
    }

    else
      return fusion::vector3<iterator, bool, scope::size_type>
        (it, true, depth);
  }
  
  fusion::vector3<const_iterator, bool, scope::size_type>
  operator[] (key_type const& name) const {
    const_iterator it = definitions.find(name),
                   end = definitions.end();

    if (it == end) {
      if (outer)
        return (*outer)[name];

      return fusion::vector3<const_iterator, bool, scope::size_type>
        (it, false, depth);
    }

    else
      return fusion::vector3<const_iterator, bool, scope::size_type>
        (it, true, depth);
  }

  template<class T>
  void define (key_type const& name, T const& val) {
    // TODO: replace with exception
    BOOST_ASSERT(definitions.find(name) == definitions.end());
    definitions.insert(value_type(name, val));
  }

  bool defined (key_type const& name) const {
    return fusion::at_c<1>((*this)[name]); 
  }
  
  bool locally_defined (key_type const& name) const {
    return definitions.count(name); 
  }

  environment* parent (void) const {
    return outer;
  }

  scope::size_type level (void) const {
    return depth;
  }

  environment* outer;
  scope::size_type depth;
  map_type definitions;
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_DE441280_828F_4B7F_B79F_5EA7766C9625

