/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_DDA86D33_4A25_47F5_9EED_7A07C5C6B16E)
#define PRANA_DDA86D33_4A25_47F5_9EED_7A07C5C6B16E

#include <prana/config.hpp>

#include <iosfwd>
#include <sstream>

#include <prana/generate/visit/json.hpp>
#include <prana/parse/parse_tree.hpp>
#include <prana/parse/grammar/json.hpp>

namespace prana {

template<class Char>
bool generate_json (parse_tree<tag::json> const& in,
                    std::basic_ostream<Char>& out)
{
  json_printer<std::basic_ostream<Char> > print(out, in.annotations());
  print(in.ast());
  return true; 
}

template<class Char>
bool generate_json (parse_tree<tag::json> const& in,
                    std::basic_string<Char>& out)
{
  std::basic_stringstream<Char> oss;
  json_printer<std::basic_stringstream<Char> > print(oss, in.annotations());
  print(in.ast());
  out = oss.str();
  return true; 
}

} // prana

#endif // PRANA_DDA86D33_4A25_47F5_9EED_7A07C5C6B16E

