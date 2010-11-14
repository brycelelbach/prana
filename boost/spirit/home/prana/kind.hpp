/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_KIND_HPP)
#define BOOST_SPIRIT_PRANA_KIND_HPP

#include <boost/utility/binary.hpp>

namespace boost {
namespace spirit {
namespace prana {

enum kind_type {
  numeric_kind     = BOOST_BINARY(0000 0001), /* |  1 | 0x01 | 001 | */
    nil_kind       = BOOST_BINARY(0001 0001), /* | 17 | 0x11 | 021 | */
    integer_kind   = BOOST_BINARY(0010 0001), /* | 33 | 0x21 | 041 | */
    floating_kind  = BOOST_BINARY(0011 0001), /* | 49 | 0x31 | 061 | */

  container_kind   = BOOST_BINARY(0000 0010), /* |  2 | 0x02 | 002 | */
    symbol_kind    = BOOST_BINARY(0001 0010), /* | 18 | 0x12 | 022 | */
    sequence_kind  = BOOST_BINARY(0010 0010), /* | 34 | 0x22 | 042 | */ 

  reference_kind   = BOOST_BINARY(0000 0100), /* |  4 | 0x04 | 004 | */ 

  record_kind      = BOOST_BINARY(0000 1000)  /* |  8 | 0x08 | 010 | */
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_KIND_HPP
