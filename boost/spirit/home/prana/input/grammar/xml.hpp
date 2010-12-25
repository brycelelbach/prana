/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INPUT_GRAMAR_XML_HPP)
#define BOOST_SPIRIT_PRANA_INPUT_GRAMAR_XML_HPP

#include <boost/spirit/include/support_utree.hpp>

#include <boost/spirit/home/prana/input/grammar/string.hpp>
#include <boost/spirit/home/prana/input/error_handler.hpp>
#include <boost/spirit/home/prana/input/save_line_pos.hpp>

#include <boost/spirit/home/prana/support/utree_nil_traits.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Iterator>
struct xml_white_space: qi::grammar<Iterator> {
  qi::rule<Iterator>
    start;

  xml_white_space (void): xml_white_space::base_type(start) {
    using standard::space;
    using standard::char_;

    start = space | ("<!--" >> *char_ >> "-->");
  }
};

template<class Iterator, class ErrorHandler = error_handler<Iterator> >
struct xml_parser:
  qi::grammar<Iterator, utree(void), xml_white_space<Iterator> >
{
  qi::rule<Iterator, utree(void), xml_white_space<Iterator> >
    start, element;

  qi::rule<Iterator, utree(void)>
    atom;

  qi::symbols<char, bool>
    boolean;

  qi::rule<Iterator, utf8_symbol_type(void)>
    symbol;

  string_parser<Iterator>
    utf8;

  phoenix::function<ErrorHandler> const
    error;
  
  save_line_pos<Iterator>
    pos;

  xml_parser (std::string const& source_file = "<string>"):
    xml_parser::base_type(start), error(ErrorHandler(source_file))
  {
    using phoenix::front;
    using standard::char_;
    using qi::lexeme;
    using qi::int_;
    using qi::lazy;
    using qi::real_parser;
    using qi::strict_real_policies;
    using qi::uint_parser;
    using qi::on_error;
    using qi::fail;
    using qi::_val;
    using qi::_1;
    using qi::_2;
    using qi::_3;
    using qi::_4;

    real_parser<double, strict_real_policies<double> > strict_double;
    uint_parser<unsigned char, 16, 2, 2> hex2;
  
    start = element.alias();

    element = pos(_val, '<') > symbol            > '>'
            > *atom
            >           "</" > lazy(front(_val)) > '>';

    atom = strict_double
         | int_
         | boolean
         | utf8
         | symbol
         | element;

    boolean.add
      ("true", true)
      ("false", false);

    std::string exclude = std::string(" <>&;\"\x01-\x1f\x7f") + '\0';
    symbol = lexeme[+(~char_(exclude))];

    start.name("xml");
    element.name("element");
    atom.name("atom");
    symbol.name("symbol");
 
    on_error<fail>(start, error(_1, _2, _3, _4));
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INPUT_GRAMAR_XML_HPP

