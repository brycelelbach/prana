/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_PP_IS_ITERATING)
  #if !defined(BOOST_SPIRIT_PRANA_VM_FUNCTION_CALL_HPP)
  #define BOOST_SPIRIT_PRANA_VM_FUNCTION_CALL_HPP

  #include <boost/preprocessor/iterate.hpp>
  #include <boost/preprocessor/repetition/enum_params.hpp>
  #include <boost/preprocessor/repetition/enum_binary_params.hpp>
  #include <boost/preprocessor/repetition/repeat.hpp>

  #if !defined(BOOST_SPIRIT_PRANA_COMPOSITE_LIMIT)
    #define BOOST_SPIRIT_PRANA_COMPOSITE_LIMIT 8
  #endif

  #define BSP_PUSH_ELEMENT(z, n, _) elements[n] = _##n;

  #define BOOST_PP_ITERATION_PARAMS_1                         \
    (3, (3, BOOST_PP_DEC(BOOST_SPIRIT_PRANA_COMPOSITE_LIMIT), \
    <boost/spirit/home/prana/vm/function_call.hpp>))          \
    /***/

  #include BOOST_PP_ITERATE()

  #undef BSP_PUSH_ELEMENT

  #endif // BOOST_SPIRIT_PRANA_VM_FUNCTION_CALL_HPP

#else // defined(BOOST_PP_IS_ITERATING)
  #define BSP_N BOOST_PP_ITERATION()

  template<BOOST_PP_ENUM_PARAMS(BSP_N, class A)>
  utree operator() (BOOST_PP_ENUM_BINARY_PARAMS(BSP_N, A, const& _)) const {
    boost::array<utree, BSP_N> elements;
    BOOST_PP_REPEAT(BSP_N, BSP_PUSH_ELEMENT, _);
    return derived()(get_range(elements));
  }

  #undef BSP_N

#endif // defined(BOOST_PP_IS_ITERATING)

