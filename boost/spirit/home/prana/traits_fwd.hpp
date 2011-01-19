/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TRAITS_FWD_HPP)
#define BOOST_SPIRIT_PRANA_TRAITS_FWD_HPP

#include <boost/mpl/has_xxx.hpp>

#include <boost/preprocessor/cat.hpp>

namespace boost {
namespace spirit {

struct utree;

template<class Tag>
struct parse_tree;

template<class Tag, class Iterator>
struct error_handler;

namespace prana {
namespace result_of {

template<class Tag, class Enable = void>
struct extract_source_location;

template<class Tag, class Enable = void>
struct extract_list_subtype;

} // result_of

namespace traits {

#define BOOST_SPIRIT_PRANA_TRAIT(name)                                     \
  BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(BOOST_PP_CAT(is_, name), name, false)  \
  /***/ 

BOOST_SPIRIT_PRANA_TRAIT(visitable)
BOOST_SPIRIT_PRANA_TRAIT(type_definition)
BOOST_SPIRIT_PRANA_TRAIT(type_registry)
BOOST_SPIRIT_PRANA_TRAIT(tag_binder)
BOOST_SPIRIT_PRANA_TRAIT(routine)

BOOST_SPIRIT_PRANA_TRAIT(arithmetic_type) 
BOOST_SPIRIT_PRANA_TRAIT(singleton_type) 
BOOST_SPIRIT_PRANA_TRAIT(dynamic_array_type) 
BOOST_SPIRIT_PRANA_TRAIT(cons_type) 

BOOST_SPIRIT_PRANA_TRAIT(parser_tag)

BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(is_heap_allocated_type, heap, false) 
BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(is_stack_allocated_type, stack, false) 

template<class Tag, class Enable = void>
struct annotations_type;

template<class Tag, class Enable = void>
struct source_type;

template<class Tag, class Enable = void>
struct default_source;

template<class Tag, class Enable = void>
struct has_source_locations;

template<class Tag, class Enable = void>
struct get_string_from_source;

template<class Tag>
std::string stringify_source (typename source_type<Tag>::type const&);

template<class Tag, class Enable = void>
struct extract_source_location_from_node;

template<class Tag>
typename prana::result_of::extract_source_location<Tag>::type
extract_source_location (utree const&, parse_tree<Tag> const&);

template<class Tag, class Enable = void>
struct has_list_subtypes;

template<class Tag, class Enable = void>
struct list_subtypes;

template<class Tag, class Enable = void>
struct extract_list_subtype_from_node;

template<class Tag>
typename prana::result_of::extract_list_subtype<Tag>::type
extract_list_subtype (utree const&, parse_tree<Tag> const&);

template<class Tag, class Iterator, class Enable = void>
struct parser_type;

template<class Tag, class Iterator, class Enable = void>
struct parser_invoker;

template<class Tag, class Iterator>
bool invoke (Iterator&, Iterator, typename parser_type<Tag, Iterator>::type,
             utree&); 

template<class Tag, class OtherTag, class Enable = void>
struct annotations_builder;

template<class Tag, class OtherTag>
void build_annotations (parse_tree<Tag>&, parse_tree<OtherTag> const&);

template<class Tag, class Enable = void>
struct utf_version;

template<class Tag, class Iterator, class Enable = void>
struct has_whitespace;

template<class Tag, class Iterator, class Enable = void>
struct whitespace_type;

template<class Tag, class Iterator, class Enable = void>
struct error_handler_type;

} // traits
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TRAITS_FWD_HPP

