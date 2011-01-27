/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_99FBA54B_F9AA_4FC6_B4BF_FE7A434D25AD)
#define BSP_99FBA54B_F9AA_4FC6_B4BF_FE7A434D25AD

#include <boost/spirit/include/support_utree.hpp>

#include <boost/spirit/home/prana/parse/grammar/string.hpp>
#include <boost/spirit/home/prana/parse/error_handler.hpp>
#include <boost/spirit/home/prana/parse/annotator.hpp>

namespace boost {
namespace spirit {
namespace prana {

///////////////////////////////////////////////////////////////////////////////
namespace tag {

struct sexpr {
  typedef mpl::string<'sexp', 'r'> name; 
  struct parser_tag;
  struct sexpr_parser_tag;
};

} // tag
 
/////////////////////////////////////////////////////////////////////////////// 
typedef mpl::set<>
  sexpr_list_subtypes;

typedef dynamic_array<fusion::vector1<source_location> >
  sexpr_annotations;

///////////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////////// 
namespace magic {

BSP_TRAIT(sexpr_parser_tag)

template<class Tag, class Iterator>
struct whitespace_type<Tag, Iterator, typename enable_if<
    is_sexpr_parser_tag<Tag>
  >::type
> {
  typedef sexpr_white_space<Iterator> type;
}; 

} // magic

///////////////////////////////////////////////////////////////////////////////
template<class Tag, class Iterator, class Enable = void>
struct sexpr_parser;

template<class Tag, class Iterator>
struct sexpr_parser<Tag, Iterator, typename enable_if<
    mpl::not_<magic::has_whitespace<Tag, Iterator> >
  >::type
> {
  BOOST_SPIRIT_ASSERT_MSG(
    (magic::has_whitespace<Tag, Iterator>::value),
    sexpr_parser_requires_whitespace_parser, (Tag, Iterator));
};

template<class Tag, class Iterator>
struct sexpr_parser<Tag, Iterator, typename enable_if<
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

  typedef annotator<Tag, Iterator>
    annotator_type; 

  typedef utf8_string_parser<Tag, Iterator>
    utf8_type;

  qi::rule<Iterator, utree(void), space_type>
    start, element, list;

  qi::rule<Iterator, utree(void)>
    atom;

  qi::rule<Iterator, int(void)>
    integer;

  // TODO: replace with bool_parser and custom policies
  qi::symbols<char, bool>
    boolean;

  qi::rule<Iterator, utf8_symbol_type(void)>
    symbol;
 
  qi::rule<Iterator, utree::nil_type(void)>
    nil_;

  qi::rule<Iterator, binary_string_type(void)>
    binary;

  utf8_type
    utf8;

  phoenix::function<error_handler_type> const
    error;

  annotator_type
    annotate;  

  sexpr_parser (source_type const& source, annotations_type& annotations):
    sexpr_parser::base_type(start, mpl::c_str<typename Tag::name>::value),
    utf8(source), error(error_handler_type(source)), annotate(annotations)
  {
    using qi::char_;
    using qi::string;
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
    using qi::_3;
    using qi::_4;

    real_parser<double, strict_real_policies<double> > real;
    uint_parser<unsigned char, 16, 2, 2> hex2;
  
    start = element.alias();

    element = atom | list;

    list = '(' > annotate(_val) > *element > ')';

    atom = nil_ 
         | real
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

    std::string name = mpl::c_str<typename Tag::name>::value;

    start.name(name);
    element.name(name + ":element");
    list.name(name + ":list");
    atom.name(name + ":atom");
    nil_.name(name + ":nil");
    integer.name(name + ":integer");
    symbol.name(name + ":symbol");
    binary.name(name + ":binary");
 
    on_error<fail>(start, error(_3, _4));
  }
};

///////////////////////////////////////////////////////////////////////////////
namespace magic {

template<class Tag, class Iterator>
struct parser_type<Tag, Iterator, typename enable_if<
    is_sexpr_parser_tag<Tag>
  >::type
> {
  typedef sexpr_parser<Tag, Iterator> type;
};

} // magic

} // prana
} // spirit
} // boost

#endif // BSP_99FBA54B_F9AA_4FC6_B4BF_FE7A434D25AD

