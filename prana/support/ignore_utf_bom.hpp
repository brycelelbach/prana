/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_E515D31E_05A2_4422_B25B_7B1DE8A4035B)
#define PRANA_E515D31E_05A2_4422_B25B_7B1DE8A4035B

#include <prana/config.hpp>

#include <string>

#include <boost/cstdint.hpp>

#include <prana/exception.hpp>

namespace prana {

template <boost::uint8_t Version, typename Enable = void>
struct ignore_utf_bom_hook;

template <>
struct ignore_utf_bom_hook<0> {
  typedef void type;

  template <typename Input>
  static type call (Input& in, std::string const& source) { }
};

template <>
struct ignore_utf_bom_hook<8> {
  typedef void type;

  template <typename Input>
  static type call (Input& in, std::string const& source) {
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

template <boost::uint8_t Version, typename Input>
inline typename ignore_utf_bom_hook<Version>::type
ignore_utf_bom (Input& in, std::string const& source)
{ return ignore_utf_bom_hook<Version>::call(in, source); }

} // prana

#endif // PHXPR_E515D31E_05A2_4422_B25B_7B1DE8A4035B

