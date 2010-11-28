/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCOPE_HPP)
#define BOOST_SPIRIT_PRANA_SCOPE_HPP

#include <cstddef>

#include <boost/range/iterator_range.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class AST>
class scope: public iterator_range<AST*> {
 public:
  scope (AST* first = 0, AST* last = 0, scope const* parent = 0);

  scope const* outer (void) const;

  std::size_t level (void) const;

 private:
  scope const* parent;
  std::size_t depth;
};

template<class AST>
scope<AST>::scope (AST* first, AST* last, scope const* parent):
  iterator_range<AST*>(first, last), parent(parent),
  depth(parent ? parent->depth + 1 : 0) { }

template<class AST>
scope<AST> const* scope<AST>::outer (void) const {
  return parent;
}

template<class AST>
std::size_t scope<AST>::level (void) const {
  return depth;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCOPE_HPP

