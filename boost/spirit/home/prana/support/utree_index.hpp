/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_SUPPORT_UTREE_INDEX_HPP)
#define BSP_SUPPORT_UTREE_INDEX_HPP

#include <boost/spirit/include/support_utree.hpp>
#include <boost/next_prior.hpp>

namespace boost {
namespace spirit {

///////////////////////////////////////////////////////////////////////////////
template<class X, class N>
inline typename X::reference index (X& x, N n) {
  return *boost::next(x.begin(), n);
}

template<class X, class N>
inline typename X::const_reference index (X const& x, N n) {
  return *boost::next(x.begin(), n);
}

template<class X>
inline typename X::reference index (X& x, utree const& n) {
  return *boost::next(x.begin(), n);
}

template<class X>
inline typename X::const_reference index (X const& x, utree const& n) {
  return *boost::next(x.begin(), n);
}

///////////////////////////////////////////////////////////////////////////////
template<class N>
inline utree at_c (utree& x, N n) {
  return *boost::next(x.begin(), n);
}

template<class N>
inline utree index (utree const& x, N n) {
  return *boost::next(x.begin(), n);
}

inline utree index (utree& x, utree const& n) {
  return *boost::next(x.begin(), n.get<int>());
}

inline utree index (utree const& x, utree const& n) {
  return *boost::next(x.begin(), n.get<int>());
}

} // spirit
} // boost

#endif // BSP_SUPPORT_UTREE_INDEX_HPP

