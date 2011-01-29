/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_TRAITS_FWD_HPP)
#define BSP_TRAITS_FWD_HPP

#include <string>

#include <boost/mpl/has_xxx.hpp>

#include <boost/preprocessor/cat.hpp>

namespace boost {
namespace spirit {

class utree;

namespace prana {

template<class Tag>
class parse_tree;

template<class Tag, class Iterator>
struct error_handler;

namespace magic {

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

template<class T, class Enable = void>
struct get_string_from_type;

template<class T>
typename get_string_from_type<T>::type stringify (T&);

template<class Tag, class Enable = void>
struct extract_source_location_from_node;

template<class Tag>
typename extract_source_location_from_node<Tag>::type
extract_source_location (utree const&, parse_tree<Tag> const&);

template<class Tag, class Enable = void>
struct has_list_subtypes;

template<class Tag, class Enable = void>
struct list_subtypes;

template<class Tag, class Enable = void>
struct extract_list_subtype_from_node;

template<class Tag>
typename extract_list_subtype_from_node<Tag>::type
extract_list_subtype (utree const&, parse_tree<Tag> const&);

template<class Tag, class Iterator, class Enable = void>
struct parser_type;

template<class Tag, class Iterator, class Enable = void>
struct parser_invoker;

template<class Tag, class Iterator>
bool invoke (Iterator&, Iterator,
             typename parser_type<Tag, Iterator>::type&, utree&); 

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

} // magic
} // prana
} // spirit
} // boost

#endif // BSP_TRAITS_FWD_HPP

