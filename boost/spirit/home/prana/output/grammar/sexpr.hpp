/*==============================================================================
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_OUTPUT_GRAMMAR_SEXPR_HPP)
#define BOOST_SPIRIT_PRANA_OUTPUT_GRAMMAR_SEXPR_HPP

#include <boost/spirit/include/support_utree.hpp>
#include <boost/spirit/include/karma.hpp>

#include <boost/spirit/home/prana/support/utree_nil_traits.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Iterator>
struct sexpr_generator:
  karma::grammar<Iterator, utree(void)>
{
  typedef iterator_range<utree::const_iterator> utree_list;

  karma::rule<Iterator, utree(void)>
    start, ref_;

  karma::rule<Iterator, utree_list(void)>
    list;

  karma::rule<Iterator, utf8_symbol_range_type(void)>
    symbol;

  karma::rule<Iterator, nil_type(void)>
    nil_;

  karma::rule<Iterator, utf8_string_range_type(void)>
    utf8;

  karma::rule<Iterator, binary_range_type(void)>
    binary;

  karma::symbols<bool, char const*>
    boolean;

  sexpr_generator (void): sexpr_generator::base_type(start) {
    using standard::char_;
    using standard::string;
    using karma::uint_generator;
    using karma::double_;
    using karma::int_;
    using karma::attr_cast;
    using karma::lit;
    using karma::right_align;

    uint_generator<unsigned char, 16> hex2;

    start = nil_
          | double_
          | int_
          | boolean
          | utf8
          | symbol
          | binary
          | list
          | ref_;
  
    ref_ = start;

    list = '(' << -(start % ' ') << ')';

    utf8 = '"' << *(&char_('"') << "\\\"" | char_) << '"';

    symbol = string;

    binary = '#' << *right_align(2, '0')[hex2] << '#';

    nil_ = attr_cast(lit("nil"));

    boolean.add
      (true, "#t")
      (false, "#f");

    start.name("sexpr");
    ref_.name("ref");
    list.name("list");
    utf8.name("string");
    symbol.name("symbol");
    binary.name("binary");
    nil_.name("nil");
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_OUTPUT_GRAMMAR_SEXPR_HPP

