/*==============================================================================
    Copyright (c) 2010 Object Modeling Designs
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INPUT_GRAMAR_JSON_HPP)
#define BOOST_SPIRIT_PRANA_INPUT_GRAMAR_JSON_HPP

#include <boost/spirit/include/support_utree.hpp>

#include <boost/spirit/home/prana/input/grammar/string.hpp>
#include <boost/spirit/home/prana/input/error_handler.hpp>
#include <boost/spirit/home/prana/input/save_line_pos.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Iterator, class ErrorHandler = error_handler<Iterator> >
struct json_parser: qi::grammar<Iterator, utree(void), standard::space_type> {
  qi::rule<Iterator, utree(void), standard::space_type>
    start, value, object, member_pair, array;

  qi::symbols<char, bool>
    boolean;

  qi::rule<Iterator, utf8_symbol(void)>
    member;

  string_parser<Iterator>
    string;

  phoenix::function<ErrorHandler> const
    error;

  json_parser (std::string const& source_file = "<string>"):
    json_parser::base_type(start), error(ErrorHandler(source_file))
  {
    using standard::char_;
    using qi::lexeme;
    using qi::omit;
    using qi::raw;
    using qi::eps;
    using qi::real_parser;
    using qi::strict_real_policies;
    using qi::int_parser;
    using qi::on_error;
    using qi::fail;
    using qi::_val;
    using qi::_1;
    using qi::_2;
    using qi::_3;
    using qi::_4;

    real_parser<double, strict_real_policies<double> > strict_double;
    int_parser<boost::intmax_t> max_int;

    start = value.alias();

    value = max_int
          | strict_double
          | boolean
          | string
          | object
          | array
          | "null";

    object %= '{'
            > omit[raw[eps] [save_line_pos(_val, _1)]]
            > (member_pair % ',')
            > '}';

    member_pair %= omit[raw[eps] [save_line_pos(_val, _1)]]
                 > member
                 > ':'
                 > value;
    
    array %= '['
           > omit[raw[eps] [save_line_pos(_val, _1)]]
           > (value % ',')
           > ']';

    boolean.add
      ("true", true)
      ("false", false);

    std::string exclude = std::string(" {}[]:\"\x01-\x1f\x7f") + '\0';
    member = lexeme[+(~char_(exclude))];
 
    start.name("json");
    value.name("value");
    object.name("object");
    member_pair.name("member_pair");
    array.name("array");
    member.name("member");
 
    on_error<fail>(start, error(_1, _2, _3, _4));
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INPUT_GRAMAR_JSON_HPP

