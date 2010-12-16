/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_PP_IS_ITERATING)
  #if !defined(BOOST_SPIRIT_PRANA_VM_COMPOSITE_CALL_HPP)
  #define BOOST_SPIRIT_PRANA_VM_COMPOSITE_CALL_HPP

  #include <boost/preprocessor/iterate.hpp>
  #include <boost/preprocessor/repetition/enum_params.hpp>
  #include <boost/preprocessor/repetition/enum_binary_params.hpp>
  #include <boost/preprocessor/repetition/repeat.hpp>

  #if !defined(BOOST_SPIRIT_PRANA_COMPOSITE_LIMIT)
    #define BOOST_SPIRIT_PRANA_COMPOSITE_LIMIT 8
  #endif

  #define BSP_PUSH_ELEMENT(z, n, _) elements.push_back(as_function(_##n));

  #define BOOST_PP_ITERATION_PARAMS_1                         \
    (3, (3, BOOST_PP_DEC(BOOST_SPIRIT_PRANA_COMPOSITE_LIMIT), \
    <boost/spirit/home/prana/vm/composite_call.hpp>))         \
    /***/

  #include BOOST_PP_ITERATE()

  #undef BSP_PUSH_ELEMENT

  #endif // BOOST_SPIRIT_PRANA_VM_COMPOSITE_CALL_HPP

#else // defined(BOOST_PP_IS_ITERATING)
  #define BSP_N BOOST_PP_ITERATION()

  template<BOOST_PP_ENUM_PARAMS(BSP_N, class A)>
  function operator() (BOOST_PP_ENUM_BINARY_PARAMS(BSP_N, A, const& _)) const {
    actor_list elements;
    BOOST_PP_REPEAT(N, SCHEME_PUSH_ELEMENT, _);
    return derived()(elements);
  }

  #undef BSP_N

#endif // defined(BOOST_PP_IS_ITERATING)

