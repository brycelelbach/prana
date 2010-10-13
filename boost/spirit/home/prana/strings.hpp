/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach
   
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_STRINGS_HPP)
#define BOOST_SPIRIT_PRANA_STRINGS_HPP

#include <boost/spirit/home/prana/strings/basic_string.hpp>
#include <boost/spirit/home/prana/strings/fast_string.hpp>

namespace boost {
namespace spirit {
namespace prana {

typedef basic_string<iterator_range<char const*>, tree_type::binary_type>
  binary_range;

typedef basic_string<std::string, tree_type::binary_type>
  binary_string;

typedef basic_string<iterator_range<char const*>, tree_type::string_type>
  utf8_string_range;

typedef basic_string<std::string, tree_type::string_type>
  utf8_string;

typedef basic_string<iterator_range<char const*>, tree_type::symbol_type>
  utf8_symbol_range;

typedef basic_string<std::string, tree_type::symbol_type>
  utf8_symbol;

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_STRINGS_HPP
