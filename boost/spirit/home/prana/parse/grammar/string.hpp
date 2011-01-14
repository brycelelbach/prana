/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INPUT_GRAMMAR_STRING_HPP)
#define BOOST_SPIRIT_PRANA_INPUT_GRAMMAR_STRING_HPP

#include <string>

#include <boost/cstdint.hpp>

#include <boost/regex/pending/unicode_iterator.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace boost {
namespace spirit {
namespace prana {

typedef boost::uint32_t uchar;

struct push_utf8_functor {
  template<class, class>
  struct result {
    typedef void type;
  };

  void operator() (std::string& utf8, uchar code_point) const {
    typedef std::back_insert_iterator<std::string> insert_iter;
    insert_iter out_iter(utf8);
    utf8_output_iterator<insert_iter> utf8_iter(out_iter);
    *utf8_iter++ = code_point;
  }
};

struct push_esc_functor {
  template<class, class>
  struct result {
    typedef void type;
  };

  void operator() (std::string& utf8, uchar c) const {
    switch (c) {
      case 'b':
        utf8 += '\b';
        break;
      case 't':
        utf8 += '\t';
        break;
      case 'n':
        utf8 += '\n';
        break;
      case 'f':
        utf8 += '\f';
        break;
      case 'r':
        utf8 += '\r';
        break;
      case '"':
        utf8 += '"';
        break;
      case '\\':
        utf8 += '\\';
        break;
    }
  }
};

template<class Iterator>
struct string_parser: qi::grammar<Iterator, std::string(void)> {
  qi::rule<Iterator, void(std::string&)>
    escaped;

  qi::rule<Iterator, std::string(void)>
    start;
  
  phoenix::function<push_utf8_functor>
    push_utf8;

  phoenix::function<push_esc_functor>
    push_esc;

  string_parser (void): string_parser::base_type (start) {
    using standard::char_;
    using qi::uint_parser;
    using qi::_val;
    using qi::_r1;
    using qi::_1;

    uint_parser<uchar, 16, 4, 4> hex4;
    uint_parser<uchar, 16, 8, 8> hex8;

    escaped
      = '\\'
      > (   ('u' > hex4)                 [push_utf8(_r1, _1)]
        |   ('U' > hex8)                 [push_utf8(_r1, _1)]
        |   char_("btnfr\\\"'")          [push_esc(_r1, _1)]
        );

    start
      = '"'
      > *(escaped(_val) | (~char_('"'))  [_val += _1])
      > '"';

    escaped.name("escaped_string");
    start.name("string");
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INPUT_GRAMMAR_STRING_HPP

