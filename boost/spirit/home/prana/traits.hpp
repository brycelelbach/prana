/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TRAITS_HPP)
#define BOOST_SPIRIT_PRANA_TRAITS_HPP

#include <boost/type_traits/is_convertible.hpp>

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/preprocessor/seq.hpp>

namespace boost {
namespace spirit {
namespace prana {

#define BOOST_SPIRIT_PRANA_DEFINE_TRAIT(_, d, trait)              \
  BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(                              \
    BOOST_PP_SEQ_ELEM(1, trait), BOOST_PP_SEQ_ELEM(0, trait), d)  \
  /***/ 

#define BOOST_SPIRIT_PRANA_DEFINE_TRAITS(d, traits)                  \
  BOOST_PP_SEQ_FOR_EACH(BOOST_SPIRIT_PRANA_DEFINE_TRAIT, d, traits)  \
  /***/

BOOST_SPIRIT_PRANA_DEFINE_TRAITS(
  false,
  ((universal_tree)          (is_universal_tree)) 
  ((type_definition)         (is_type_definition)) 
  ((type_registry)           (is_type_registry))
  ((tag_binder)              (is_tag_binder)) 
  ((implementation_functor)  (is_implementation_functor)) 
  ((interface_functor)       (is_interface_functor)))

template<class Definition, class T>
struct is_convertible_to_data_definition:
  mpl::bool_<is_convertible<typename Definition::data_type, T>::value> { };

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TRAITS_HPP

