/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SUPPORT_UTREE_REST_HPP)
#define BOOST_SPIRIT_PRANA_SUPPORT_UTREE_REST_HPP

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {

template<class X>
inline X rest (X& x) {
  typename X::iterator i = x.begin(); ++i;
  return X(i, x.end());
}

template<class X>
inline X rest (X const& x) {
  typename X::const_iterator i = x.begin(); ++i;
  return X(i, x.end());
}

inline utree rest (utree& x) {
  utree::iterator i = x.begin(); ++i;
  return utree(utree::range(i, x.end()), shallow);
}

inline utree rest (utree const& x) {
  utree::const_iterator i = x.begin(); ++i;
  return utree(utree::const_range(i, x.end()), shallow);
}

} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SUPPORT_UTREE_REST_HPP

