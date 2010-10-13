/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_REST_HPP)
#define BOOST_SPIRIT_PRANA_REST_HPP

#include <boost/spirit/home/prana/utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

utree rest (utree& x) {
  utree::iterator i = x.begin(); ++i;
  return utree(utree::range(i, x.end()), shallow);
}

utree rest (utree const& x) {
  utree::const_iterator i = x.begin(); ++i;
  return utree(utree::const_range(i, x.end()), shallow);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_REST_HPP

