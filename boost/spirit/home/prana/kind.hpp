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

enum kind {
  nil_kind       =              000,
  reference_kind = BOOST_BINARY(001), 
  symbol_kind    = BOOST_BINARY(010),
  numeric_kind   = BOOST_BINARY(011), 
  sequence_kind  = BOOST_BINARY(100),  
  set_kind       = BOOST_BINARY(101),  
  record_kind    = BOOST_BINARY(110)
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_KIND_HPP
