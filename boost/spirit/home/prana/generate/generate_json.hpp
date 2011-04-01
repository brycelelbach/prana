/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_OUTPUT_GENERATE_JSON_HPP)
#define BSP_OUTPUT_GENERATE_JSON_HPP

#include <sstream>

#include <boost/spirit/include/karma_generate.hpp>
#include <boost/spirit/include/karma_char.hpp>
#include <boost/spirit/include/karma_list.hpp>
#include <boost/spirit/include/support_ostream_iterator.hpp>

#include <boost/spirit/home/prana/generate/visit/json.hpp>
#include <boost/spirit/home/prana/generate/generate_json_fwd.hpp>

namespace boost {
namespace spirit {
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
} // spirit
} // boost

#endif // BSP_OUTPUT_GENERATE_JSON_HPP

