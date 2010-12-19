/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_GET_SYMBOL_HPP)
#define BOOST_SPIRIT_PRANA_VM_GET_SYMBOL_HPP

#include <boost/spirit/include/support_utree.hpp>

#include <boost/spirit/home/prana/vm/exception.hpp>

namespace boost {
namespace spirit {
namespace prana {

inline std::string get_symbol (utree const& s) {
  if (s.which() != utree_type::symbol_type)
    throw expected_identifier(s);

  utf8_symbol_range symbol = s.get<utf8_symbol_range>();
  return std::string(symbol.begin(), symbol.end());
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_GET_SYMBOL_HPP

