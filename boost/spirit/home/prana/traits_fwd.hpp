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

namespace prana {
namespace result_of {

template<class Tag, class Enable = void>
struct extract_source_location;

template<class Tag, class Enable = void>
struct extract_list_subtype;

} // result_of

namespace traits {

#define BOOST_SPIRIT_PRANA_TRAIT(name)                                \
  BOOST_MPL_HAS_TRAIT_NAMED_DEF(BOOST_PP_CAT(is, name), name, false)  \
  /***/ 

BOOST_SPIRIT_PRANA_TRAIT(visitable)
BOOST_SPIRIT_PRANA_TRAIT(type_definition)
BOOST_SPIRIT_PRANA_TRAIT(type_registry)
BOOST_SPIRIT_PRANA_TRAIT(tag_binder)
BOOST_SPIRIT_PRANA_TRAIT(routine)

template<class Tag, class Enable = void>
struct annotations_type;

template<class Tag, class Enable = void>
struct has_source_locations;

template<class Tag, class Enable = void>
struct extract_source_location_from_node;

template<class PT, class Enable = void>
typename prana::result_of<typename PT::tag>::type
extract_source_location (utree const&, PT const&);

template<class Tag, class Enable = void>
struct has_list_subtypes;

template<class Tag, class Enable = void>
struct list_subtypes;

template<class Tag, class Enable = void>
struct extract_list_subtype_from_node;

template<class PT, class Enable = void>
typename prana::result_of<typename PT::tag>::type
extract_list_subtype (utree const&, PT const&);

} // traits
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TRAITS_FWD_HPP

