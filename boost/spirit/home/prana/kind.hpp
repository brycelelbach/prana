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
  // fundamental kinds
  nil_kind       =              00000000,
  reference_kind = BOOST_BINARY(00000001), 
  symbol_kind    = BOOST_BINARY(00000010),
  record_kind    = BOOST_BINARY(00000100),

  // abstract kinds
  numeric_kind   = BOOST_BINARY(00001000),
  container_kind = BOOST_BINARY(00010000),

  // numeric kinds
  unsigned_kind  = BOOST_BINARY(00101000),
  signed_kind    = BOOST_BINARY(01001000),
  floating_kind  = BOOST_BINARY(10001000),

  // container kinds
  sequence_kind  = BOOST_BINARY(00110000),  
  array_kind     = BOOST_BINARY(01010000),  
  unique_kind    = BOOST_BINARY(10010000)  
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_KIND_HPP
