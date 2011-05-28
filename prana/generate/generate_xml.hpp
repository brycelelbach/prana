/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_99BA1145_183B_4652_B220_F9FFCADB7C85)
#define PRANA_99BA1145_183B_4652_B220_F9FFCADB7C85

#include <prana/config.hpp>

#include <iosfwd>
#include <sstream>

#include <prana/generate/visit/xml.hpp>
#include <prana/parse/parse_tree.hpp>
#include <prana/parse/grammar/xml.hpp>

namespace prana {

template <typename Char>
bool generate_xml (parse_tree<tag::xml> const& in,
                   std::basic_ostream<Char>& out)
{
  xml_printer<std::basic_ostream<Char> > print(out, in.annotations());
  print(in.ast());
  return true; 
}

template <typename Char>
bool generate_xml (parse_tree<tag::xml> const& in, std::basic_string<Char>& out)
{
  std::basic_stringstream<Char> oss;
  xml_printer<std::basic_stringstream<Char> > print(oss, in.annotations());
  print(in.ast());
  out = oss.str();
  return true; 
}

template <typename Char>
std::basic_ostream<Char>&
operator<< (std::basic_ostream<Char>& out, parse_tree<tag::xml> const& in) {
  xml_printer<std::basic_ostream<Char> > print(out, in.annotations());
  print(in.ast());
  return out;
}

} // prana

#endif // PRANA_99BA1145_183B_4652_B220_F9FFCADB7C85

