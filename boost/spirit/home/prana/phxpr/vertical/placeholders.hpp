/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_PP_IS_ITERATING)
  #if !defined(BSP_5EF2DB57_8627_43CC_A183_583025C1B819)
  #define BSP_5EF2DB57_8627_43CC_A183_583025C1B819

  #include <boost/spirit/home/prana/config.hpp>
  #include <boost/preprocessor/iterate.hpp>
  #include <boost/preprocessor/repetition/enum_params.hpp>
  #include <boost/preprocessor/repetition/enum_binary_params.hpp>
  #include <boost/preprocessor/repetition/repeat.hpp>

  #if !defined BSP_MAKE_PLACEHOLDER 
    #error BSP_MAKE_PLACEHOLDER must be defined
  #endif
  
  #if defined(BSP_THIS_HEADER)
    #undef BSP_THIS_HEADER
  #endif

  #define BSP_THIS_HEADER                                      \
    <boost/spirit/home/prana/phxpr/vertical/placeholders.hpp>  \
    /***/

  #define BOOST_PP_ITERATION_PARAMS_1           \
    (3, (3, BSP_ARITY_LIMIT, BSP_THIS_HEADER))  \
    /***/

  #include BOOST_PP_ITERATE()

  #undef BSP_MAKE_PLACEHOLDER
  #undef BSP_THIS_HEADER

  #endif // BSP_5EF2DB57_8627_43CC_A183_583025C1B819

#else // defined(BOOST_PP_IS_ITERATING)
  BSP_MAKE_PLACEHOLDER(BOOST_PP_ITERATION());

#endif // defined(BOOST_PP_IS_ITERATING)

