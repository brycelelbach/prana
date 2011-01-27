/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_GET_SYMBOL_HPP)
#define BSP_PHXPR_GET_SYMBOL_HPP

#include <boost/spirit/include/support_utree.hpp>

#include <boost/spirit/home/prana/phxpr/exception.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

inline std::string get_symbol (utree const& s) {
  if (s.which() != utree_type::symbol_type)
    throw expected_identifier(s);

  utf8_symbol_range_type symbol = s.get<utf8_symbol_range_type>();
  return std::string(symbol.begin(), symbol.end());
}

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_GET_SYMBOL_HPP

