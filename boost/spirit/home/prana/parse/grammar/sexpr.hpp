/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INPUT_GRAMAR_SEXPR_HPP)
#define BOOST_SPIRIT_PRANA_INPUT_GRAMAR_SEXPR_HPP

#include <boost/spirit/include/support_utree.hpp>

#include <boost/spirit/home/prana/parse/grammar/string.hpp>
#include <boost/spirit/home/prana/parse/error_handler.hpp>
#include <boost/spirit/home/prana/parse/save_line_pos.hpp>

#include <boost/spirit/home/prana/support/utree_nil_traits.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Iterator>
struct sexpr_white_space: qi::grammar<Iterator> {
  qi::rule<Iterator>
    start;

  sexpr_white_space (void): sexpr_white_space::base_type(start) {
    using standard::space;
    using standard::char_;
    using qi::eol;

    start = space | (';' >> *(char_ - eol) >> eol);
  }
};

template<class Iterator, class ErrorHandler = error_handler<Iterator> >
struct sexpr_parser:
  qi::grammar<Iterator, utree(void), sexpr_white_space<Iterator> >
{
  qi::rule<Iterator, utree(void), sexpr_white_space<Iterator> >
    start, element, list;

  qi::rule<Iterator, utree(void)>
    atom;

  qi::rule<Iterator, int(void)>
    integer;

  qi::symbols<char, bool>
    boolean;

  qi::rule<Iterator, utf8_symbol_type(void)>
    symbol;
 
  qi::rule<Iterator, utree::nil_type(void)>
    nil_;

  qi::rule<Iterator, binary_string_type(void)>
    binary;

  string_parser<Iterator>
    utf8;

  phoenix::function<ErrorHandler> const
    error;
  
  save_line_pos<Iterator>
    pos;

  sexpr_parser (std::string const& source_file = "<string>"):
    sexpr_parser::base_type(start), error(ErrorHandler(source_file))
  {
    using standard::char_;
    using standard::string;
    using qi::lexeme;
    using qi::hex;
    using qi::oct;
    using qi::no_case;
    using qi::attr;
    using qi::real_parser;
    using qi::strict_real_policies;
    using qi::uint_parser;
    using qi::on_error;
    using qi::fail;
    using qi::int_;
    using qi::lit;
    using qi::_val;
    using qi::_1;
    using qi::_2;
    using qi::_3;
    using qi::_4;

    real_parser<double, strict_real_policies<double> > strict_double;
    uint_parser<unsigned char, 16, 2, 2> hex2;
  
    start = element.alias();

    element = atom | list;

    list = pos(_val, '(') > *element > ')';

    atom = nil_ 
         | strict_double
         | integer
         | boolean
         | utf8
         | symbol
         | binary;

    nil_ = "nil" >> attr(spirit::nil); 

    integer = lexeme[ no_case["#x"] >  hex]
            | lexeme[ no_case["#o"] >> oct]
            | lexeme[-no_case["#d"] >> int_];

    boolean.add
      ("#t", true)
      ("true", true)
      ("#f", false)
      ("false", false);

    std::string exclude = std::string(" ();\"\x01-\x1f\x7f") + '\0';
    symbol = lexeme[+(~char_(exclude))];

    binary = lexeme['#' > *hex2 > '#'];

    start.name("sexpr");
    element.name("element");
    list.name("list");
    atom.name("atom");
    nil_.name("nil");
    integer.name("integer");
    symbol.name("symbol");
    binary.name("binary");
 
    on_error<fail>(start, error(_1, _2, _3, _4));
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INPUT_GRAMAR_SEXPR_HPP

