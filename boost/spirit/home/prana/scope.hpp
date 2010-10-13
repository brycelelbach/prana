/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach
   
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCOPE_HPP)
#define BOOST_SPIRIT_PRANA_SCOPE_HPP

#include <boost/spirit/home/prana/utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

class scope: public boost::iterator_range<utree*> {
 public:
  scope (utree* first = 0, utree* last = 0, scope const* parent = 0):
    boost::iterator_range<utree*>(first, last),
    parent(parent),
    depth(parent ? (parent->depth + 1) : 0) { }

  scope const* outer (void) const { return parent; }
  int level (void) const { return depth; }

 private:
  scope const* parent;
  int depth;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCOPE_HPP
