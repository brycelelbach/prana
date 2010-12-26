/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TRAITS_HPP)
#define BOOST_SPIRIT_PRANA_TRAITS_HPP

#include <boost/type_traits/is_convertible.hpp>

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/preprocessor/seq.hpp>

namespace boost {
namespace spirit {
namespace prana {

#define BOOST_SPIRIT_PRANA_TRAIT(_, d, trait)                     \
  BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(                              \
    BOOST_PP_SEQ_ELEM(1, trait), BOOST_PP_SEQ_ELEM(0, trait), d)  \
  /***/ 

#define BOOST_SPIRIT_PRANA_TRAITS(d, traits)                  \
  BOOST_PP_SEQ_FOR_EACH(BOOST_SPIRIT_PRANA_TRAIT, d, traits)  \
  /***/

//[concept_traits
BOOST_SPIRIT_PRANA_TRAITS(
  false,
  ((visitable)       (is_visitable)) 
  ((type_definition) (is_type_definition)) 
  ((type_registry)   (is_type_registry))
  ((tag_binder)      (is_tag_binder)) 
  ((routine)         (is_routine))) 
//]

template<class Definition, class T>
struct is_convertible_to_data_definition:
  mpl::bool_<is_convertible<typename Definition::data_type, T>::value> { };

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TRAITS_HPP

