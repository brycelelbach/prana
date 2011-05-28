/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_4398DB6B_0575_44F9_B7C4_3F0CD7DB255F)
#define PRANA_4398DB6B_0575_44F9_B7C4_3F0CD7DB255F

#include <limits>

#include <boost/mpl/string.hpp>
#include <boost/mpl/order.hpp>

#include <sheol/adt/dynamic_array.hpp>

#include <prana/parse/error_handler.hpp>
#include <prana/parse/subtype_annotator.hpp>

namespace prana {

/////////////////////////////////////////////////////////////////////////////// 
namespace tag {

struct xml {
  typedef mpl::string<'xml'> name; 
  struct parser_tag;
  struct xml_parser_tag;
};

struct xml_element { };

struct xml_empty_element { };

struct xml_instruction { };

struct xml_children { };

struct xml_attribute { };

struct xml_attributes { };

} // tag
 
/////////////////////////////////////////////////////////////////////////////// 
typedef mpl::set<
  tag::xml_element,
  tag::xml_empty_element,
  tag::xml_instruction,
  tag::xml_children,
  tag::xml_attribute,
  tag::xml_attributes
> xml_list_subtypes;

typedef sheol::adt::dynamic_array<fusion::vector2<source_location, long> >
  xml_annotations;

///////////////////////////////////////////////////////////////////////////////
template<class Iterator>
struct xml_white_space: qi::grammar<Iterator> {
  qi::rule<Iterator>
    start, comment;

  xml_white_space (void): xml_white_space::base_type(start) {
    using spirit::standard::space;
    using spirit::standard::char_;

    start = space | comment;

    // [15] Comment ::= '<!--' ((Char - '-') | ('-' (Char - '-')))* '-->'
    comment = "<!--" >> *((char_ - '-') | ('-' >> (char_ - '-'))) >> "-->";
  }
};

/////////////////////////////////////////////////////////////////////////////// 
namespace magic {

PRANA_TRAIT(xml_parser_tag)

template<class Tag>
struct annotations_type<Tag,
  typename boost::enable_if<is_xml_parser_tag<Tag> >::type
> {
  typedef xml_annotations type; 
};

template<class Tag>
struct has_list_subtypes<Tag,
  typename boost::enable_if<is_xml_parser_tag<Tag> >::type
>: mpl::true_ { };

template<class Tag>
struct list_subtypes<Tag, typename boost::enable_if<is_xml_parser_tag<Tag> >::type>:
  xml_list_subtypes { };

template<class Tag>
struct extract_list_subtype_from_node<Tag,
  typename boost::enable_if<is_xml_parser_tag<Tag> >::type
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

template<class Tag, class Iterator>
struct whitespace_type<Tag, Iterator, typename boost::enable_if<
    is_xml_parser_tag<Tag>
  >::type
> {
  typedef xml_white_space<Iterator> type;
}; 

} // magic

/////////////////////////////////////////////////////////////////////////////// 
struct xml_element:
  mpl::order<xml_list_subtypes, tag::xml_element>::type { };

struct xml_empty_element:
  mpl::order<xml_list_subtypes, tag::xml_empty_element>::type { };

struct xml_instruction:
  mpl::order<xml_list_subtypes, tag::xml_instruction>::type { };

struct xml_children:
  mpl::order<xml_list_subtypes, tag::xml_children>::type { };

struct xml_attribute:
  mpl::order<xml_list_subtypes, tag::xml_attribute>::type { };

struct xml_attributes:
  mpl::order<xml_list_subtypes, tag::xml_attributes>::type { };

/////////////////////////////////////////////////////////////////////////////// 
template<class Tag, class Iterator, class Enable = void>
struct xml_parser;

template<class Tag, class Iterator>
struct xml_parser<Tag, Iterator, typename boost::enable_if<
    mpl::not_<magic::has_whitespace<Tag, Iterator> >
  >::type
> {
  BOOST_SPIRIT_ASSERT_MSG(
    (magic::has_whitespace<Tag, Iterator>::value),
    xml_parser_requires_whitespace_parser, (Tag, Iterator));
};

template<class Tag, class Iterator>
struct xml_parser<Tag, Iterator, typename boost::enable_if<
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

  qi::rule<Iterator, utree(void), space_type>
    start, document, pi, empty, element, atom, value, attribute;

  qi::rule<Iterator, int(void)>
    integer;

  qi::rule<Iterator, utree::list_type(void), space_type>
    attributes, children;

  qi::rule<Iterator, spirit::utf8_symbol_type(void)>
    name;

  qi::rule<Iterator, spirit::utf8_string_type(void)>
    char_data, attr_string, pi_string;

  phoenix::function<error_handler_type> const
    error;
  
  annotator_type
    annotate;

  xml_parser (source_type const& source, annotations_type& annotations):
    xml_parser::base_type(start, mpl::c_str<typename Tag::name>::value),
    error(error_handler_type(source)), annotate(annotations)
  {
    using qi::char_;
    using qi::no_skip;
    using qi::omit;
    using qi::bool_;
    using qi::int_;
    using qi::hex;
    using qi::oct;
    using qi::lexeme;
    using qi::no_case;
    using qi::real_parser;
    using qi::strict_real_policies;
    using qi::on_error;
    using qi::fail;
    using qi::_val;
    using qi::_3;
    using qi::_4;

    real_parser<double, strict_real_policies<double> > real;
 
    start = *pi >> document;

    document = empty | element;

    pi = "<?" >> name >> pi_string >> "?>"
       >> annotate(_val, xml_instruction::value);

    empty = '<' >> name >> attributes >> "/>"
          >> annotate(_val, xml_empty_element::value);
 
    element = '<' >> name >> attributes >> '>'
            >> annotate(_val, xml_element::value)
            >> children >> "</" >> omit[name] >> '>';

    atom = real
         | integer
         | bool_
         | char_data
         | document;

    value = real
          | integer
          | bool_
          | attr_string;

    integer = lexeme[ no_case["0x"] >  hex]
            | lexeme[ no_case["0o"] >> oct]
            | lexeme[-no_case["0d"] >> int_];

    attributes = annotate(_val, xml_attributes::value) >> *attribute;

    attribute = annotate(_val, xml_attribute::value)
              >> name >> '=' >> '"' >> value >> '"';

    children = annotate(_val, xml_children::value) >> *atom;

    std::string name_start_char = ":A-Z_a-z";
    std::string name_char = ":A-Z_a-z-.0-9";

    name = char_(name_start_char) >> *char_(name_char);
    
    char_data = +(~char_("<&") - "</");
    
    pi_string = *(~char_ - "?>");
    
    attr_string = +~char_("<&\"");

    std::string name_ = mpl::c_str<typename Tag::name>::value;

    start.name        (name_);
    document.name     (name_ + ":document");
    pi.name           (name_ + ":processing-instruction");
    empty.name        (name_ + ":empty-element");
    element.name      (name_ + ":element");
    atom.name         (name_ + ":atom");
    value.name        (name_ + ":value");
    attribute.name    (name_ + ":attribute");
    attributes.name   (name_ + ":attributes");
    children.name     (name_ + ":children");
    name.name         (name_ + ":name");
    char_data.name    (name_ + ":character-data");
    attr_string.name  (name_ + ":attribute-string");
    pi_string.name    (name_ + ":processing-instruction-string");
 
    on_error<fail>(start, error(_3, _4));
  }
};

///////////////////////////////////////////////////////////////////////////////
namespace magic {

template<class Tag, class Iterator>
struct parser_type<Tag, Iterator, typename boost::enable_if<
    is_xml_parser_tag<Tag>
  >::type
> {
  typedef xml_parser<Tag, Iterator> type;
};

} // magic

} // prana

#endif // PRANA_4398DB6B_0575_44F9_B7C4_3F0CD7DB255F

