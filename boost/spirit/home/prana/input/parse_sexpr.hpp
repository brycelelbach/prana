/*==============================================================================
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INPUT_PARSE_SEXPR_HPP)
#define BOOST_SPIRIT_PRANA_INPUT_PARSE_SEXPR_HPP

#include <iostream>
#include <string>

#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>
#include <boost/spirit/include/qi_parse.hpp>

#include <boost/spirit/home/prana/input/grammar/sexpr.hpp>
#include <boost/spirit/home/prana/input/parse_sexpr_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Char>
bool parse_sexpr (std::basic_istream<Char>& in, utree& out,
                  std::string const& source)
{
  typedef spirit::basic_istream_iterator<Char>
    stream_iterator_type;
  
  typedef line_pos_iterator<stream_iterator_type>
    iterator_type;

  // no white space skipping in the stream!
  in.unsetf(std::ios::skipws);

  sexpr_parser<iterator_type> p(source);
  sexpr_white_space<iterator_type> ws;

  stream_iterator_type sfirst(in);
  stream_iterator_type slast;
  iterator_type first(sfirst);
  iterator_type last(slast);

  return qi::phrase_parse(first, last, p, ws, out);
}

template<class Char>
bool parse_sexpr_list (std::basic_istream<Char>& in, utree& out,
                       std::string const& source)
{
  typedef spirit::basic_istream_iterator<Char>
    stream_iterator_type;
  
  typedef line_pos_iterator<stream_iterator_type>
    iterator_type;

  // no white space skipping in the stream!
  in.unsetf(std::ios::skipws);

  sexpr_parser<iterator_type> p(source);
  sexpr_white_space<iterator_type> ws;

  stream_iterator_type sfirst(in);
  stream_iterator_type slast;
  iterator_type first(sfirst);
  iterator_type last(slast);

  return qi::phrase_parse(first, last, *p, ws, out);
}

template<class Range>
typename disable_if<is_base_of<std::ios_base, Range>, bool>::type
parse_sexpr (Range const& in, utree& out, std::string const& source) {
  typedef line_pos_iterator<typename Range::const_iterator>
    iterator_type;

  sexpr_parser<iterator_type> p(source);
  sexpr_white_space<iterator_type> ws;

  iterator_type first(in.begin());
  iterator_type last(in.end());

  return qi::phrase_parse(first, last, p, ws, out);
}

template<class Range>
typename disable_if<is_base_of<std::ios_base, Range>, bool>::type
parse_sexpr_list (Range const& in, utree& out, std::string const& source) {
  typedef line_pos_iterator<typename Range::const_iterator>
    iterator_type;

  sexpr_parser<iterator_type> p(source);
  sexpr_white_space<iterator_type> ws;

  iterator_type first(in.begin());
  iterator_type last(in.end());

  return qi::phrase_parse(first, last, *p, ws, out);
}

bool parse_sexpr (utree const& in, utree& out,
                  std::string const& source)
{
  return parse_sexpr(in.get<utf8_string_range_type>(), out, source);
}

bool parse_sexpr_list (utree const& in, utree& out,
                       std::string const& source)
{
  return parse_sexpr_list(in.get<utf8_string_range_type>(), out, source);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INPUT_PARSE_SEXPR_HPP

