/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_ACF39D8D_FC51_4EDB_BCD9_63343723D0A6)
#define PRANA_ACF39D8D_FC51_4EDB_BCD9_63343723D0A6

#include <prana/include/utree.hpp>

namespace prana {

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
  return utree(utree::range(i, x.end()));
}

inline utree rest (utree const& x) {
  utree::const_iterator i = x.begin(); ++i;
  return utree(utree::const_range(i, x.end()));
}

} // prana 

#endif // PRANA_ACF39D8D_FC51_4EDB_BCD9_63343723D0A6

