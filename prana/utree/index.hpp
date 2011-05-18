/*==============================================================================
    Copyright (c) 2010-2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_4C4D3C1B_958F_45C7_A639_C2CAEF74B94F)
#define PRANA_4C4D3C1B_958F_45C7_A639_C2CAEF74B94F

#include <prana/config.hpp>

#include <boost/next_prior.hpp>

#include <prana/include/utree.hpp>

namespace prana {

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
inline utree index (utree& x, N n) {
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

} // prana

#endif // PRANA_4C4D3C1B_958F_45C7_A639_C2CAEF74B94F

