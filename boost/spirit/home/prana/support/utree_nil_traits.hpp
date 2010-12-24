/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SUPPORT_UTREE_NIL_TRAITS_HPP)
#define BOOST_SPIRIT_PRANA_SUPPORT_UTREE_NIL_TRAITS_HPP

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace traits { 

template<>
struct transform_attribute<nil_type, unused_type, qi::domain> {
  typedef unused_type type;

  static unused_type pre (nil_type&) { return unused_type(); }
  static void post (nil_type&, unused_type) { }
  static void fail (nil_type&) { }
};

template<>
struct transform_attribute<nil_type, unused_type, karma::domain> {
  typedef unused_type type;

  static unused_type pre (nil_type&) { return unused_type(); }
};

} // traits
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SUPPORT_UTREE_NIL_TRAITS_HPP

