/*==============================================================================
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INPUT_PARSE_JSON_HPP)
#define BOOST_SPIRIT_PRANA_INPUT_PARSE_JSON_HPP

#include <iostream>
#include <string>

#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>
#include <boost/spirit/include/qi_parse.hpp>

#include <boost/spirit/home/prana/input/grammar/json.hpp>
#include <boost/spirit/home/prana/input/parse_json_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Char>
bool parse_json (std::basic_istream<Char>& in, utree& out,
                 std::string const& source)
{
  typedef spirit::basic_istream_iterator<Char>
    stream_iterator_type;
  
  typedef line_pos_iterator<stream_iterator_type>
    iterator_type;

  // no white space skipping in the stream!
  in.unsetf(std::ios::skipws);

  json_parser<iterator_type> p(source);

  stream_iterator_type sfirst(in);
  stream_iterator_type slast;
  iterator_type first(sfirst);
  iterator_type last(slast);

  using standard::space;

  return qi::phrase_parse(first, last, p, space, out);
}

template<class Char>
bool parse_json_array (std::basic_istream<Char>& in, utree& out,
                       std::string const& source)
{
  typedef spirit::basic_istream_iterator<Char>
    stream_iterator_type;
  
  typedef line_pos_iterator<stream_iterator_type>
    iterator_type;

  // no white space skipping in the stream!
  in.unsetf(std::ios::skipws);

  json_parser<iterator_type> p(source);

  stream_iterator_type sfirst(in);
  stream_iterator_type slast;
  iterator_type first(sfirst);
  iterator_type last(slast);

  using standard::space;

  return qi::phrase_parse(first, last, p.value % ',', space, out);
}

template<class Char>
bool parse_json_object (std::basic_istream<Char>& in, utree& out,
                        std::string const& source)
{
  typedef spirit::basic_istream_iterator<Char>
    stream_iterator_type;
  
  typedef line_pos_iterator<stream_iterator_type>
    iterator_type;

  // no white space skipping in the stream!
  in.unsetf(std::ios::skipws);

  json_parser<iterator_type> p(source);

  stream_iterator_type sfirst(in);
  stream_iterator_type slast;
  iterator_type first(sfirst);
  iterator_type last(slast);

  using standard::space;

  return qi::phrase_parse(first, last, p.member_pair % ',', space, out);
}

template<class Range>
typename disable_if<is_base_of<std::ios_base, Range>, bool>::type
parse_json (Range const& in, utree& out, std::string const& source) {
  typedef line_pos_iterator<typename Range::const_iterator>
    iterator_type;

  json_parser<iterator_type> p(source);

  iterator_type first(in.begin());
  iterator_type last(in.end());

  using standard::space;

  return qi::phrase_parse(first, last, p, space, out);
}

template<class Range>
typename disable_if<is_base_of<std::ios_base, Range>, bool>::type
parse_json_array (Range const& in, utree& out, std::string const& source) {
  typedef line_pos_iterator<typename Range::const_iterator>
    iterator_type;

  json_parser<iterator_type> p(source);

  iterator_type first(in.begin());
  iterator_type last(in.end());

  using standard::space;

  return qi::phrase_parse(first, last, p.value % ',', space, out);
}

template<class Range>
typename disable_if<is_base_of<std::ios_base, Range>, bool>::type
parse_json_object (Range const& in, utree& out, std::string const& source) {
  typedef line_pos_iterator<typename Range::const_iterator>
    iterator_type;

  json_parser<iterator_type> p(source);

  iterator_type first(in.begin());
  iterator_type last(in.end());

  using standard::space;

  return qi::phrase_parse(first, last, p.member_pair % ',', space, out);
}

bool parse_json (utree const& in, utree& out, std::string const& source) {
  return parse_json(in.get<utf8_string_range_type>(), out, source);
}

bool parse_json_array (utree const& in, utree& out,
                       std::string const& source)
{
  return parse_json_array(in.get<utf8_string_range_type>(), out, source);
}

bool parse_json_object (utree const& in, utree& out,
                        std::string const& source)
{
  return parse_json_object(in.get<utf8_string_range_type>(), out, source);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INPUT_PARSE_JSON_HPP

