/*==============================================================================
    Copyright (c) 2010 Object Modeling Designs
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INPUT_GRAMAR_JSON_HPP)
#define BOOST_SPIRIT_PRANA_INPUT_GRAMAR_JSON_HPP

#include <boost/spirit/include/support_utree.hpp>

#include <boost/spirit/home/prana/parse/grammar/string.hpp>
#include <boost/spirit/home/prana/parse/error_handler.hpp>
#include <boost/spirit/home/prana/parse/save_line_pos.hpp>

namespace boost {
namespace spirit {
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
typedef mpl::set<tag::json_object, tag::json_array, tag::json_member_pair>
  json_list_subtypes;

typedef dynamic_array<fusion::vector2<source_location, long> >
  json_annotations;

/////////////////////////////////////////////////////////////////////////////// 
namespace traits {

BOOST_SPIRIT_PRANA_TRAIT(json_parser_tag)

template<class Tag>
struct annotations_type<Tag,
  typename enable_if<is_json_parser_tag<Tag> >::type
> {
  typedef json_annotations type; 
};

template<class Tag>
struct has_list_subtypes<Tag,
  typename enable_if<is_json_parser_tag<Tag> >::type
>: mpl::true_ { };

template<class Tag>
struct list_subtypes<Tag, typename enable_if<is_json_parser_tag<Tag> >::type>:
  json_list_subtypes { };

template<class Tag>
struct extract_list_subtype_from_node<Tag,
  typename enable_if<is_json_parser_tag<Tag> >::type
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

} // traits

/////////////////////////////////////////////////////////////////////////////// 
struct json_object:
  mpl::order<json_list_subtypes, tag::json_object> { };

struct json_array:
  mpl::order<json_list_subtypes, tag::json_array> { };

struct json_member_pair:
  mpl::order<json_list_subtypes, tag::json_member_pair> { };

///////////////////////////////////////////////////////////////////////////////
template<class Tag>
struct push_json_annotation {
  typedef typename traits::annotations_type<Tag>::type
    annotations_type;

  template<class, class, class>
  struct result {
    typedef void type;
  };

  annotations_type& annotations;

  push_json_annotation (annotations_type& annotations_):
    annotations(annotations_) { }

  template<class Range>
  void operator() (utree& ast, long type, Range const& rng) const {
    typedef json_annotations::value_type value_type;
    typedef annotations_type::size_type size_type

    value_type v(get_location(rng.begin()), type);

    annotations.push_back(v);  
    size_type n = annotations.size() - 1;

    BOOST_ASSERT(n <= (boost::detail::numeric_limits<short>::max)());
    ast.tag(n);
  }
};

template<class Tag, class Iterator>
struct json_annotator: qi::grammar<Iterator, void(utree&, long)> {
  typedef typename traits::annotations_type<Tag>::type
    annotations_type;

  typedef push_json_annotation<Tag>
    pusher_type;

  qi::rule<Iterator, void(utree&, long)>
    start;
  
  phoenix::function<pusher_type> const
    push;

  json_annotator (annotations_type& annotations):
    json_annotator::base_type(start), push(pusher_type(annotations))
  {
    using qi::omit;
    using qi::raw;
    using qi::eps;
    using qi::_1;
    using qi::_r1;
    using qi::_r2;

    start = omit[raw[eps] [push(_r1, _r2, _1)]];
  }
};

///////////////////////////////////////////////////////////////////////////////
template<class Tag, class Iterator, class Enable = void>
struct json_parser;

template<class Tag, class Iterator>
struct json_parser<Tag, Iterator, typename enable_if<
    mpl::not_<traits::has_whitespace<Tag, Iterator> >
  >::type
> {
  BOOST_SPIRIT_ASSERT_MSG(
    (traits::has_whitespace<Tag, Iterator>),
    json_parser_requires_whitespace_parser, (Tag, Iterator));
};

template<class Tag, class Iterator>
struct json_parser<Tag, Iterator, typename enable_if<
    traits::has_whitespace<Tag, Iterator>
  >::type
>: qi::grammar<
  Iterator, utree(void), typename whitespace_type<Tag, Iterator>::type
> {
  typedef typename traits::source_type<Tag>::type
    source_type;

  typedef typename traits::annotations_type<Tag>::type
    annotations_type;

  typedef typename traits::error_handler_type<Tag, Iterator>::type
    error_handler_type;

  typedef typename traits::whitespace_type<Tag, Iterator>::type
    space_type;

  typedef json_annotator<Tag, Iterator>
    annotator_type; 

  typedef utf8_string_parser<Tag, Iterator>
    utf8_type;

  qi::rule<Iterator, utree(void), space_type>
    start, value;
  
  qi::rule<Iterator, utree::list_type(void), space_type>
    member_pair, object, array;

  qi::rule<Iterator, utf8_symbol_type(void)>
    member;

  qi::rule<Iterator, utf8_symbol_type(void), space_type>
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
    json_parser::base_type(start, mpl::c_str<Tag::name>::value), utf8(source),
    error(error_handler_type(source)), annotate(annotations)
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

    real_parser<double, strict_real_policies<double> > strict_double;
        
    as<utf8_symbol_type> as_symbol;

    start = value.alias();

    value = null
          | strict_double
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

    std::string name = mpl::c_str<Tag::name>::value;
 
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

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INPUT_GRAMAR_JSON_HPP

