/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_SUPPORT_UTREE_NIL_TRAITS_HPP)
#define BSP_SUPPORT_UTREE_NIL_TRAITS_HPP

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace traits { 

template<>
struct transform_attribute<utree::nil_type, unused_type, karma::domain> {
  typedef unused_type type;

  static unused_type pre (utree::nil_type&) { return unused_type(); }
};

} // traits
} // spirit
} // boost

#endif // BSP_SUPPORT_UTREE_NIL_TRAITS_HPP

