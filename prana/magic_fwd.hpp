/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_2D1DB4C5_5EE3_49F7_B9CB_0A2C5E81324C)
#define PRANA_2D1DB4C5_5EE3_49F7_B9CB_0A2C5E81324C

#include <prana/config.hpp>

#include <string>

namespace prana {

template <typename Tag>
class parse_tree;

template <typename Tag, typename Iterator>
struct error_handler;

namespace magic {

template <typename Tag, typename Enable = void>
struct annotations_type;

template <typename Tag, typename Enable = void>
struct source_type;

template <typename Tag, typename Enable = void>
struct default_source;

template <typename Tag, typename Enable = void>
struct has_source_locations;

template <typename Tag, typename Enable = void>
struct get_string_from_source;

template <typename Tag>
std::string stringify_source (typename source_type<Tag>::type const&);

template <typename T, typename Enable = void>
struct get_string_from_type;

template <typename T>
typename get_string_from_type<T>::type stringify (T&);

template <typename Tag, typename Enable = void>
struct extract_source_location_from_node;

template <typename Tag>
typename extract_source_location_from_node<Tag>::type
extract_source_location (utree const&, parse_tree<Tag> const&);

template <typename Tag, typename Enable = void>
struct has_list_subtypes;

template <typename Tag, typename Enable = void>
struct list_subtypes;

template <typename Tag, typename Enable = void>
struct extract_list_subtype_from_node;

template <typename Tag>
typename extract_list_subtype_from_node<Tag>::type
extract_list_subtype (utree const&, parse_tree<Tag> const&);

template <typename Tag, typename Iterator, typename Enable = void>
struct parser_type;

template <typename Tag, typename Iterator, typename Enable = void>
struct parser_invoker;

template <typename Tag, typename Iterator>
bool invoke (Iterator&, Iterator,
             typename parser_type<Tag, Iterator>::type&, utree&); 

template <typename Tag, typename OtherTag, typename Enable = void>
struct annotations_builder;

template <typename Tag, typename OtherTag>
void build_annotations (parse_tree<Tag>&, parse_tree<OtherTag> const&);

template <typename Tag, typename Enable = void>
struct utf_version;

template <typename Tag, typename Iterator, typename Enable = void>
struct has_whitespace;

template <typename Tag, typename Iterator, typename Enable = void>
struct whitespace_type;

template <typename Tag, typename Iterator, typename Enable = void>
struct error_handler_type;

} // magic
} // prana

#endif // PRANA_2D1DB4C5_5EE3_49F7_B9CB_0A2C5E81324C

