/*==============================================================================
    Copyright (c) 2010 Object Modeling Designs
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INPUT_GRAMAR_JSON_HPP)
#define BOOST_SPIRIT_PRANA_INPUT_GRAMAR_JSON_HPP

#include <boost/spirit/include/support_utree.hpp>

#include <boost/spirit/home/prana/input/grammar/string.hpp>
#include <boost/spirit/home/prana/input/error_handler.hpp>
#include <boost/spirit/home/prana/input/save_line_pos.hpp>

#include <boost/spirit/home/prana/support/utree_nil_traits.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Iterator, class ErrorHandler = error_handler<Iterator> >
struct json_parser: qi::grammar<Iterator, utree(void), standard::space_type> {
  qi::rule<Iterator, utree(void), standard::space_type>
    start, value;
  
  qi::rule<Iterator, utree::list_type(void), standard::space_type>
    member_pair, object, array;

  qi::symbols<char, bool>
    boolean;

  qi::rule<Iterator, utf8_symbol_type(void)>
    member;

  qi::rule<Iterator, utf8_symbol_type(void), standard::space_type>
    empty_object, empty_array;

  qi::rule<Iterator, utree::nil_type(void)>
    null;

  string_parser<Iterator>
    utf8;

  phoenix::function<ErrorHandler> const
    error;
  
  save_line_pos<Iterator>
    pos;

  json_parser (std::string const& source_file = "<string>"):
    json_parser::base_type(start), error(ErrorHandler(source_file))
  {
    using standard::char_;
    using qi::lexeme;
    using qi::as;
    using qi::real_parser;
    using qi::strict_real_policies;
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
        
    typedef as<utf8_symbol_type> as_symbol_type;
    as_symbol_type const as_symbol = as_symbol_type();

    start = value.alias();

    value = null
          | strict_double
          | int_
          | boolean
          | utf8
          | object
          | array
          | empty_object
          | empty_array; 
    
    null = qi::attr_cast(lit("null"));

    object %= pos(_val, '{') >> (member_pair % ',') > '}';

    member_pair %= pos(_val, '"') > as_symbol[member] > '"' > ':' > value;
    
    array %= pos(_val, '[') >> (value % ',') > ']';

    boolean.add
      ("true", true)
      ("false", false);

    std::string exclude = std::string(" {}[]:\"\x01-\x1f\x7f") + '\0';
    member = lexeme[+(~char_(exclude))];

    empty_object = char_('{') > char_('}');
    
    empty_array = char_('[') > char_(']');
 
    start.name("json");
    value.name("value");
    null.name("null");
    object.name("object");
    member_pair.name("member_pair");
    array.name("array");
    member.name("member");
    empty_object.name("empty_object");
    empty_array.name("empty_array");
 
    on_error<fail>(start, error(_1, _2, _3, _4));
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INPUT_GRAMAR_JSON_HPP

