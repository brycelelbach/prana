////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2009-2011 Bryce Lelbach, Joel de Guzman, Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PRANA_F8E22779_56A3_4EB8_B9B3_D3D1E891CD68)
#define PRANA_F8E22779_56A3_4EB8_B9B3_D3D1E891CD68

#include <prana/config.hpp>

#include <ostream>

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {

inline std::ostream&
operator<< (std::ostream& out, utree const& ut);

} // spirit
} // boost

#endif // PRANA_F8E22779_56A3_4EB8_B9B3_D3D1E891CD68

