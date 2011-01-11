/*==============================================================================
    Copyright (c) 2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SOURCE_LOCATION_HPP)
#define BOOST_SPIRIT_PRANA_SOURCE_LOCATION_HPP

#include <iosfwd>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/spirit/include/adapt_struct.hpp>
#include <boost/spirit/include/vector.hpp>

#include <boost/spirit/include/karma_nonterminal.hpp>
#include <boost/spirit/include/karma_stream.hpp>
#include <boost/spirit/include/karma_generate.hpp>
#include <boost/spirit/include/karma_uint.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Iterator>
struct location_generator:
  karma::grammar<Iterator, fusion::vector2<std::size_t, std::size_t>
{ 
  karma::rule<Iterator, fusion::vector2<std::size_t, std::size_t>(void)>
    start;

  karma::rule<Iterator, std::size_t(void)>
    component;

  location_generator (void): location_generator::base_type(start) {
    using karma::omit;
    using karma::uint_generator;

    uint_generator<std::size_t, 10> size_t_;

    start = '(' << component << ' ' << component << ')';

    component = omit[size_t_(-1)] << '?' | size_t_;
  }
};

struct source_location {
  typedef std::size_t value_type;

  value_type line;
  value_type column;

  std::string get_line (void) const {
    try {
      using karma::generate;

      typedef std::back_insert_iterator<std::string> iterator; 

      std::string r;
      iterator sink(r);
      location_generator<iterator> g;

      if (!generate(sink, g.component, line))
        return "?";
      else
        return r;
    } catch (...) {
      return "?";
    }
  } 
  
  std::string get_column (void) const {
    try {
      using karma::generate;

      typedef std::back_insert_iterator<std::string> iterator; 

      std::string r;
      iterator sink(r);
      location_generator<iterator> g;

      if (!generate(sink, g.component, line))
        return "?";
      else
        return r;
    } catch (...) {
      return "?";
    }
  }

  std::string get (void) const {
    try {
      using karma::generate;

      typedef std::back_insert_iterator<std::string> iterator; 

      std::string r;
      iterator sink(r);
      location_generator<iterator> g;

      if (!generate(sink, g, *this))
        return "(? ?)";
      else
        return r;
    } catch (...) {
      return "(? ?)";
    }
  }
};

template<class Char, class traits>
inline operator<< (std::basic_ostream<Char, Traits>& out, source_location loc) {
  try {
    using karma::format;
    using karma::ostream_iterator;

    typedef ostream_iterator<Char, Char, Traits iterator;

    location_generator<iterator> g;

    out << format(g, loc);
  } catch (...) {
    if (out.good())
      out << "(? ?)";
    else
      throw;
  }
} 

BOOST_FUSION_ADAPT_STRUCT(
  boost::spirit::prana::employee,
  (std::size_t, line)
  (std::size_t, column))

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SOURCE_LOCATION_HPP
