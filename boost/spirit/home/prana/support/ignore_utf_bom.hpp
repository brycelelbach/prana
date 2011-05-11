/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_SUPPORT_IGNORE_UTF8_BOM_HPP)
#define BSP_SUPPORT_IGNORE_UTF8_BOM_HPP

#include <ios>
#include <istream>

#include <boost/spirit/home/prana/exception.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<unsigned char Version, class Enable = void>
struct ignore_utf_bom;

template<>
struct ignore_utf_bom<0> {
  typedef void type;

  template<class Input>
  static type call (Input& in, std::string const& source) {
    // no-op
  }
};

template<>
struct ignore_utf_bom<8> {
  typedef void type;

  static type call (std::istream& in, std::string const& source) {
    char c = in.peek();

    if (c == '\xef') {
      char s[3];
      in >> s[0] >> s[1] >> s[2];
      s[3] = '\0';
    
      if (s != std::string("\xef\xbb\xbf"))
        throw unexpected_character_data(source);
    }
  }
};

} // prana
} // spirit
} // boost

#endif // BSP_SUPPORT_IGNORE_UTF8_BOM_HPP

