/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
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
    start, comment;

  xml_white_space (void): xml_white_space::base_type(start) {
    using standard::space;
    using standard::char_;

    start = space | comment;

    // [15] Comment ::= '<!--' ((Char - '-') | ('-' (Char - '-')))* '-->'
    comment = "<!--" >> *((char_ - '-') | ('-' >> (char_ - '-'))) >> "-->";
  }
};

template<class Iterator, class ErrorHandler = error_handler<Iterator> >
struct xml_parser:
  qi::grammar<Iterator, utree(void), xml_white_space<Iterator> >
{
  qi::rule<Iterator, utree(void), xml_white_space<Iterator> >
    start, document, pi, empty, element, atom, value, attribute;

  qi::rule<Iterator, utree::list_type(void), xml_white_space<Iterator> >
    attributes, children;

  qi::rule<Iterator, utf8_symbol_type(void)>
    name;

  qi::rule<Iterator, utf8_string_type(void)>
    char_data, attr_string, pi_string;

  phoenix::function<ErrorHandler> const
    error;
  
  save_line_pos<Iterator>
    pos;

  xml_parser (std::string const& source_file = "<string>"):
    xml_parser::base_type(start), error(ErrorHandler(source_file))
  {
    using phoenix::front;
    using standard::char_;
    using qi::no_skip;
    using qi::omit;
    using qi::bool_;
    using qi::int_;
    using qi::real_parser;
    using qi::strict_real_policies;
    using qi::on_error;
    using qi::fail;
    using qi::_val;
    using qi::_1;
    using qi::_2;
    using qi::_3;
    using qi::_4;

    typedef real_parser<double, strict_real_policies<double> > real_type;
    real_type const real = real_type();
 
    start = *pi >> document;

    document = empty | element;

    pi       = pos(_val, '<') >> no_skip['?'] >> name >> pi_string >> "?>";

    empty    = pos(_val, '<') >> name >> attributes >> "/>";
 
    element  = pos(_val, '<') >> name >> attributes >> '>'
            >> children
            >>           "</" >> omit[name] >>         '>';

    atom = real
         | int_
         | bool_
         | char_data
         | document;

    value = real
          | int_
          | bool_
          | attr_string;

    attributes = *attribute;

    attribute = pos.epsilon(_val) >> name >> '=' >> '"' >> value >> '"';

    children = pos.epsilon(_val) >> *atom;

    std::string name_start_char = ":A-Z_a-z";
    std::string name_char = ":A-Z_a-z-.0-9";

    name = char_(name_start_char) >> *char_(name_char);
    
    char_data = +(~char_("<&") - "</");
    
    pi_string = *(~char_ - "?>");
    
    attr_string = +~char_("<&\"");

    start.name("xml");
    element.name("element");
    atom.name("atom");
    children.name("children");
    name.name("name");
    char_data.name("char_data");
 
    on_error<fail>(start, error(_1, _2, _3, _4));
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INPUT_GRAMAR_XML_HPP

