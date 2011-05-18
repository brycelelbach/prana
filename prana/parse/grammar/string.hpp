/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_07EDB260_F086_44EE_9353_B37608849EFF)
#define PRANA_07EDB260_F086_44EE_9353_B37608849EFF

#include <prana/config.hpp>

#include <string>

#include <boost/mpl/string.hpp>

#include <boost/cstdint.hpp>

#include <boost/regex/pending/unicode_iterator.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_function.hpp>

#include <prana/magic.hpp>

namespace prana {

typedef boost::uint32_t uchar;

///////////////////////////////////////////////////////////////////////////////
struct push_utf8_functor {
  template<class, class>
  struct result {
    typedef void type;
  };

  void operator() (std::string& utf8, uchar code_point) const {
    typedef std::back_insert_iterator<std::string> insert_iter;
    insert_iter out_iter(utf8);
    boost::utf8_output_iterator<insert_iter> utf8_iter(out_iter);
    *utf8_iter++ = code_point;
  }
};

///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
template<class Tag, class Iterator>
struct utf8_string_parser: qi::grammar<Iterator, std::string(void)> {
  typedef typename magic::source_type<Tag>::type
    source_type;

  typedef typename magic::error_handler_type<Tag, Iterator>::type
    error_handler_type;

  qi::rule<Iterator, void(std::string&)>
    escaped;

  qi::rule<Iterator, std::string(void)>
    start;
  
  phoenix::function<push_utf8_functor> const
    push_utf8;

  phoenix::function<push_esc_functor> const
    push_esc;
  
  phoenix::function<error_handler_type> const
    error;

  utf8_string_parser (source_type const& source_):
    utf8_string_parser::base_type(start,
      std::string(mpl::c_str<typename Tag::name>::value) + ":utf8-string"),
    error(error_handler_type(source_))
  {
    using qi::char_;
    using qi::uint_parser;
    using qi::on_error;
    using qi::fail;
    using qi::_val;
    using qi::_r1;
    using qi::_1;
    using qi::_3;
    using qi::_4;

    uint_parser<uchar, 16, 4, 4> hex4;
    uint_parser<uchar, 16, 8, 8> hex8;

    escaped
      = '\\'
      > ( ('u' > hex4)        [push_utf8(_r1, _1)]
        | ('U' > hex8)        [push_utf8(_r1, _1)]
        | char_("btnfr\\\"'") [push_esc(_r1, _1)]
        );

    start
      = '"'
      > *(escaped(_val) | (~char_('"')) [_val += _1])
      > '"';

    std::string name = mpl::c_str<typename Tag::name>::value;

    start.name(name + ":utf8-string");
    escaped.name(name + ":escaped-utf8-string");
    
    on_error<fail>(start, error(_3, _4));
  }
};

} // prana

#endif // PRANA_07EDB260_F086_44EE_9353_B37608849EFF

