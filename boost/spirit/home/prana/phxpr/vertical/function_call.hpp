/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_PP_IS_ITERATING)
  #if !defined(BSP_09BF1CD0_F704_4E6A_A4F7_DB586C1BAD55)
  #define BSP_09BF1CD0_F704_4E6A_A4F7_DB586C1BAD55

  #include <boost/spirit/home/prana/config.hpp>
  #include <boost/preprocessor/iterate.hpp>
  #include <boost/preprocessor/cat.hpp>
  #include <boost/preprocessor/dec.hpp>
  #include <boost/preprocessor/inc.hpp>
  #include <boost/preprocessor/repetition/enum_params.hpp>
  #include <boost/preprocessor/repetition/enum_binary_params.hpp>
  #include <boost/preprocessor/repetition/repeat.hpp>

  #if !defined(BSP_PUSH_ELEMENT)
    #define BSP_PUSH_ELEMENT(n)                                 \
      elements[n] = as_utree(BOOST_PP_CAT(_, BOOST_PP_DEC(n)))  \
    /***/
  #endif

  #if !defined(BSP_RETURN_FUNCTION)
    #define BSP_RETURN_FUNCTION derived().eval
  #endif
  
  #if defined(BSP_THIS_HEADER)
    #undef BSP_THIS_HEADER
  #endif

  #define BSP_THIS_HEADER                                       \
    <boost/spirit/home/prana/phxpr/vertical/function_call.hpp>  \
    /***/

  #define BOOST_PP_ITERATION_PARAMS_1           \
    (3, (3, BSP_ARITY_LIMIT, BSP_THIS_HEADER))  \
    /***/

  #include BOOST_PP_ITERATE()

  #undef BSP_PUSH_ELEMENT
  #undef BSP_RETURN_FUNCTION
  #undef BSP_THIS_HEADER

  #endif // BSP_09BF1CD0_F704_4E6A_A4F7_DB586C1BAD55

#elif BOOST_PP_ITERATION_DEPTH() == 1 
  #define BSP_N BOOST_PP_FRAME_ITERATION(1)

  template<BOOST_PP_ENUM_PARAMS(BSP_N, class A)>
  utree operator() (BOOST_PP_ENUM_BINARY_PARAMS(BSP_N, A, const& _)) const {
    boost::array<utree, BOOST_PP_INC(BSP_N)> elements;
    elements[0] = derived().clone();   
 
    #define BOOST_PP_ITERATION_PARAMS_2 \
      (3, (1, BSP_N, BSP_THIS_HEADER))  \
      /***/

    #include BOOST_PP_ITERATE()

    return BSP_RETURN_FUNCTION(get_range(elements));
  }

  #undef BSP_N

#elif BOOST_PP_ITERATION_DEPTH() == 2
    BSP_PUSH_ELEMENT(BOOST_PP_FRAME_ITERATION(2));

#endif // defined(BOOST_PP_IS_ITERATING)

