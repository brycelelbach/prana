/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SUPPORT_HALF_T_HPP)
#define BOOST_SPIRIT_PRANA_SUPPORT_HALF_T_HPP

#include <limits.h>

#include <boost/cstdint.hpp>
#include <boost/integer.hpp>

namespace boost {
namespace spirit {
namespace prana {

typedef int_t<(sizeof(intmax_t) * CHAR_BIT) / 2>::exact inthalf_t;
typedef uint_t<(sizeof(uintmax_t) * CHAR_BIT) / 2>::exact uinthalf_t;

} // prana
} // spirit 
} // boost 

#endif // BOOST_SPIRIT_PRANA_SUPPORT_HALF_T_HPP 

