/*==============================================================================
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_82B08A89_C8DD_4E3F_8579_874DE74AE40D)
#define PRANA_82B08A89_C8DD_4E3F_8579_874DE74AE40D

#include <prana/include/utree.hpp>

#include <boost/spirit/include/karma.hpp>

namespace prana {

template<class Iterator>
struct sexpr_generator: karma::grammar<Iterator, utree(void)>
{
  typedef boost::iterator_range<utree::const_iterator> utree_list;

  karma::rule<Iterator, utree(void)>
    start, ref_;

  karma::rule<Iterator, utree_list(void)>
    list;

  karma::rule<Iterator, spirit::utf8_symbol_range_type(void)>
    symbol;

  karma::rule<Iterator, utree::nil_type(void)>
    nil_;

  karma::rule<Iterator, spirit::utf8_string_range_type(void)>
    utf8;

  karma::rule<Iterator, spirit::binary_range_type(void)>
    binary;

  // TODO: replace with bool_generator and custom bool policies
  karma::symbols<bool, char const*>
    boolean;

  sexpr_generator (void): sexpr_generator::base_type(start) {
    using karma::char_;
    using karma::string;
    using karma::uint_generator;
    using karma::double_;
    using karma::omit;
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

    nil_ = eps << "nil"; 
    
    boolean.add
      (true, "#t")
      (false, "#f");

    // TODO: Switch to new component naming scheme.
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

#endif // PRANA_82B08A89_C8DD_4E3F_8579_874DE74AE40D

