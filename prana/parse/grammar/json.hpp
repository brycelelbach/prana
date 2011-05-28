/*==============================================================================
    Copyright (c) 2010 Object Modeling Designs
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_3F8BC114_2C2D_4BA3_9181_B2E9D9A22675)
#define PRANA_3F8BC114_2C2D_4BA3_9181_B2E9D9A22675

#include <limits>

#include <boost/mpl/order.hpp>

#include <sheol/adt/dynamic_array.hpp>

#include <prana/parse/grammar/string.hpp>
#include <prana/parse/error_handler.hpp>
#include <prana/parse/subtype_annotator.hpp>

namespace prana {

/////////////////////////////////////////////////////////////////////////////// 
namespace tag {

struct json {
  typedef mpl::string<'json'> name; 
  struct parser_tag;
  struct json_parser_tag;
};

struct json_object { };

struct json_array { };

struct json_member_pair { };

} // tag
 
/////////////////////////////////////////////////////////////////////////////// 
typedef mpl::set<
  tag::json_object,
  tag::json_array,
  tag::json_member_pair
> json_list_subtypes;

typedef sheol::adt::dynamic_array<fusion::vector2<source_location, long> >
  json_annotations;

/////////////////////////////////////////////////////////////////////////////// 
namespace magic {

PRANA_TRAIT(json_parser_tag)

template<class Tag>
struct annotations_type<Tag,
  typename boost::enable_if<is_json_parser_tag<Tag> >::type
> {
  typedef json_annotations type; 
};

template<class Tag>
struct has_list_subtypes<Tag,
  typename boost::enable_if<is_json_parser_tag<Tag> >::type
>: mpl::true_ { };

template<class Tag>
struct list_subtypes<Tag, typename boost::enable_if<is_json_parser_tag<Tag> >::type>:
  json_list_subtypes { };

template<class Tag>
struct extract_list_subtype_from_node<Tag,
  typename boost::enable_if<is_json_parser_tag<Tag> >::type
> {
  typedef int type; 

  static type call (utree const& ut, parse_tree<Tag> const& pt) {
    using fusion::at_c;
    switch (ut.which()) {
      case utree_type::reference_type:
        return call(ut.deref());
      case utree_type::range_type:
      case utree_type::list_type:
        return at_c<1>(pt.annotations()[ut.tag()]);
      default:
        return -1; 
    }
  }
};

} // magic

/////////////////////////////////////////////////////////////////////////////// 
struct json_object:
  mpl::order<json_list_subtypes, tag::json_object>::type { };

struct json_array:
  mpl::order<json_list_subtypes, tag::json_array>::type { };

struct json_member_pair:
  mpl::order<json_list_subtypes, tag::json_member_pair>::type { };

///////////////////////////////////////////////////////////////////////////////
template<class Tag, class Iterator, class Enable = void>
struct json_parser;

template<class Tag, class Iterator>
struct json_parser<Tag, Iterator, typename boost::enable_if<
    mpl::not_<magic::has_whitespace<Tag, Iterator> >
  >::type
> {
  BOOST_SPIRIT_ASSERT_MSG(
    (magic::has_whitespace<Tag, Iterator>::value),
    json_parser_requires_whitespace_parser, (Tag, Iterator));
};

template<class Tag, class Iterator>
struct json_parser<Tag, Iterator, typename boost::enable_if<
    magic::has_whitespace<Tag, Iterator>
  >::type
>: qi::grammar<
  Iterator, utree(void), typename magic::whitespace_type<Tag, Iterator>::type
> {
  typedef typename magic::source_type<Tag>::type
    source_type;

  typedef typename magic::annotations_type<Tag>::type
    annotations_type;

  typedef typename magic::error_handler_type<Tag, Iterator>::type
    error_handler_type;

  typedef typename magic::whitespace_type<Tag, Iterator>::type
    space_type;

  typedef subtype_annotator<Tag, Iterator>
    annotator_type; 

  typedef utf8_string_parser<Tag, Iterator>
    utf8_type;

  qi::rule<Iterator, utree(void), space_type>
    start, value;
  
  qi::rule<Iterator, utree::list_type(void), space_type>
    member_pair, object, array;

  qi::rule<Iterator, spirit::utf8_symbol_type(void)>
    member;

  qi::rule<Iterator, spirit::utf8_symbol_type(void), space_type>
    empty_object, empty_array;

  qi::rule<Iterator, utree::nil_type(void)>
    null;

  utf8_type
    utf8;

  phoenix::function<error_handler_type> const
    error;
  
  annotator_type
    annotate;

  json_parser (source_type const& source, annotations_type& annotations):
    json_parser::base_type(start, mpl::c_str<typename Tag::name>::value),
    utf8(source), error(error_handler_type(source)), annotate(annotations)
  {
    using qi::char_;
    using qi::lexeme;
    using qi::as;
    using qi::attr;
    using qi::real_parser;
    using qi::strict_real_policies;
    using qi::on_error;
    using qi::fail;
    using qi::int_;
    using qi::bool_;
    using qi::lit;
    using qi::_val;
    using qi::_3;
    using qi::_4;

    real_parser<double, strict_real_policies<double> > real;
        
    as<spirit::utf8_symbol_type> as_symbol;

    start = value.alias();

    value = null
          | real
          | int_
          | bool_
          | utf8
          | object
          | array
          | empty_object
          | empty_array; 
    
    null = "null" >> attr(spirit::nil); 

    object %= '{' >> (member_pair % ',') > '}'
            > annotate(_val, json_object::value);

    member_pair %= '"' > as_symbol[member] > '"' > ':' > value
                 > annotate(_val, json_member_pair::value);
    
    array %= '[' >> (value % ',') > ']'
           > annotate(_val, json_array::value);

    std::string exclude = std::string(" {}[]:\"\x01-\x1f\x7f") + '\0';
    member = lexeme[+(~char_(exclude))];

    empty_object = char_('{') > char_('}');
    
    empty_array = char_('[') > char_(']');

    std::string name = mpl::c_str<typename Tag::name>::value;
 
    start.name(name);
    value.name(name + ":value");
    null.name(name + ":null");
    object.name(name + ":object");
    member_pair.name(name + ":member-pair");
    array.name(name + ":array");
    member.name(name + ":member");
    empty_object.name(name + ":empty-object");
    empty_array.name(name + ":empty-array");
 
    on_error<fail>(start, error(_3, _4));
  }
};

///////////////////////////////////////////////////////////////////////////////
namespace magic {

template<class Tag, class Iterator>
struct parser_type<Tag, Iterator, typename boost::enable_if<
    is_json_parser_tag<Tag>
  >::type
> {
  typedef json_parser<Tag, Iterator> type;
};

} // magic

} // prana

#endif // PRANA_3F8BC114_2C2D_4BA3_9181_B2E9D9A22675

