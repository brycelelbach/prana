/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INDEX_HPP)
#define BOOST_SPIRIT_PRANA_INDEX_HPP

#include <cstddef>

namespace boost {
namespace spirit {
namespace prana {

struct indexer {
  template<typename, typename> struct result;

  template<typename Node, typename I> struct result<Node*, I> {
    typedef typename Node::value_type& type;
  };
  
  template<typename Node, typename I> struct result<Node const*, I> {
    typedef typename Node::value_type const& type;
  };
  
  template<typename Node>
  typename Node::value_type& operator() (Node* node, std::size_t i) const {
    for (; i > 0; --i) node = node->next;
    return node->val;
  }
  
  template<typename Node>
  typename Node::value_type& operator() (Node const* node, std::size_t i) const {
    for (; i > 0; --i) node = node->next;
    return node->val;
  }
};

template<typename Node>
typename indexer::result<Node, std::size_t>::type index (
  Node node, std::size_t i
) {
  indexer x;
  return x(node, i);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INDEX_HPP
