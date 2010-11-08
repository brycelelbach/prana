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
  numeric_kind     = BOOST_BINARY(00000001),
    nil_kind       = BOOST_BINARY(00100001),
    bool_kind      = BOOST_BINARY(01000001),
    integer_kind   = BOOST_BINARY(01100001),
    floating_kind  = BOOST_BINARY(10000001),

  container_kind   = BOOST_BINARY(00000010),
    symbol_kind    = BOOST_BINARY(00100010),
    sequence_kind  = BOOST_BINARY(01000010),  

  reference_kind   = BOOST_BINARY(00000100), 

  record_kind      = BOOST_BINARY(00001000)
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_KIND_HPP
