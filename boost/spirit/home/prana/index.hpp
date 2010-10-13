/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INDEX_HPP)
#define BOOST_SPIRIT_PRANA_INDEX_HPP

#include <boost/spirit/home/prana/utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

utree& index (dllist<utree>::node* node, std::size_t i) {
  for (; i > 0; --i) node = node->next;
  return node->val;
}

utree const& index (dllist<utree>::node const* node, std::size_t i) {
  for (; i > 0; --i) node = node->next;
  return node->val;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INDEX_HPP
