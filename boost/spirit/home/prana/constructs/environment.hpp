/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach
   
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ENVIRONMENT_HPP)
#define BOOST_SPIRIT_PRANA_ENVIRONMENT_HPP

#include <boost/spirit/home/prana/adt/irange.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Tree>
class environment: public irange<Tree*> {
 public:
  environment (Tree* first = 0, Tree* last = 0, environment const* parent = 0):
    parent(parent),
    depth(parent ? (parent->depth + 1) : 0)
  {
    construct(first, last);
  }

  environment const* outer (void) const { return parent; }
  int level (void) const { return depth; }

 private:
  environment const* parent;
  int depth;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ENVIRONMENT_HPP
