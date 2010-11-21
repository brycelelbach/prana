/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TRAITS_HPP)
#define BOOST_SPIRIT_PRANA_TRAITS_HPP

#include <boost/mpl/has_xxx.hpp>

#include <boost/preprocessor/seq.hpp>

namespace boost {
namespace spirit {
namespace prana {

#define BOOST_SPIRIT_PRANA_DEF_TRAIT(_, d, trait)                 \
  BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(                              \
    BOOST_PP_SEQ_ELEM(1, trait), BOOST_PP_SEQ_ELEM(0, trait), d)  \
  /***/ 

#define BOOST_SPIRIT_PRANA_DEF_TRAITS(d, traits)                  \
  BOOST_PP_SEQ_FOR_EACH(BOOST_SPIRIT_PRANA_DEF_TRAIT, d, traits)  \
  /***/

BOOST_SPIRIT_PRANA_DEF_TRAITS(false,
  ((symbolic_expression)     (is_symbolic_expression)) 
  ((type_definition)         (is_type_definition)) 
  ((type_registry)           (is_type_registry))
  ((implementation_functor)  (is_implementation_functor)) 
  ((interface_functor)       (is_interface_functor)))

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TRAITS_HPP

