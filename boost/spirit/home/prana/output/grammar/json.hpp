/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_OUTPUT_GRAMMAR_JSON_HPP)
#define BOOST_SPIRIT_PRANA_OUTPUT_GRAMMAR_JSON_HPP

#include <boost/spirit/include/support_utree.hpp>
#include <boost/spirit/include/karma.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Iterator>
struct json_generator:
  karma::grammar<Iterator, utree(void)>
{
  typedef iterator_range<utree::const_iterator> utree_list;

  karma::rule<Iterator, utree(void)>
    start, ref_, member;

  karma::rule<Iterator, utree_list(void)>
    array, member_pair, object;

  karma::rule<Iterator, utf8_symbol_range_type(void)>
    key, empty_object, empty_array;

  karma::rule<Iterator, nil_type(void)>
    null;

  karma::rule<Iterator, utf8_string_range_type(void)>
    utf8;

  karma::symbols<bool, char const*>
    boolean;

  json_generator (void): json_generator::base_type(start) {
    using standard::char_;
    using karma::string;
    using karma::double_;
    using karma::int_;
    using karma::eps;

    start = null
          | empty_object
          | empty_array
          | double_
          | int_
          | boolean
          | utf8
          | object
          | array
          | ref_;
  
    ref_ = start;

    array = '[' << (start % ", ") << ']';

    member_pair = member << ": " << start;  

    object = '{' << ((member_pair % ", ") | eps(false)) << '}'; 

    member = key | eps(false);

    key = '"' << *(&char_('"') << "\\\"" | char_) << '"';
    
    utf8 = '"' << *(&char_('"') << "\\\"" | char_) << '"';

    empty_object = string("{}");
    
    empty_array = string("[]"); 
    
    null = attr_cast(lit("null"));

    boolean.add
      (true, "true")
      (false, "false");

    start.name("json");
    ref_.name("ref");
    array.name("array");
    member_pair.name("member_pair");
    object.name("object");
    member.name("member");
    key.name("key");
    utf8.name("string");
    null.name("null");
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_OUTPUT_GRAMMAR_JSON_HPP

