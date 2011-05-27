////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !BOOST_PP_IS_ITERATING
  #if !defined(PRANA_31603375_516A_4C0F_BC98_28014C16B7BC)
  #define PRANA_31603375_516A_4C0F_BC98_28014C16B7BC

  #include <prana/include/utree.hpp>

  #include <boost/preprocessor/iteration/iterate.hpp>
  #include <boost/preprocessor/repetition/enum_params.hpp>
  #include <boost/preprocessor/repetition/repeat.hpp>
  #include <boost/preprocessor/cat.hpp>

  namespace prana {
  
  utree make_list (void) {
    utree::list_type ut;
    return ut;
  }

  #define BOOST_PP_ITERATION_PARAMS_1                         \
    (3, (1, PRANA_ARITY_LIMIT, <prana/utree/make_list.hpp>))  \
    /***/ 

  #include BOOST_PP_ITERATE()

  } // prana

  #endif // PRANA_31603375_516A_4C0F_BC98_28014C16B7BC
#else
  #define PRANA_N BOOST_PP_ITERATION()
  #define PRANA_PUSH_BACK(z, n, text) ut.push_back(utree(BOOST_PP_CAT(t, n)));

  template <BOOST_PP_ENUM_PARAMS(PRANA_N, typename T)>
  utree make_list (BOOST_PP_ENUM_BINARY_PARAMS(PRANA_N, T, const& t)) {
    utree::list_type ut;
    BOOST_PP_REPEAT(PRANA_N, PRANA_PUSH_BACK, _)
    return ut;
  } 

  #undef PRANA_PUSH_BACK
  #undef PRANA_N
#endif

