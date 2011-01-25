/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !BOOST_PP_IS_ITERATING

  #if !defined(BSP_ADT_TUPLE_HPP)
  #define BSP_ADT_TUPLE_HPP

  #include <boost/mpl/at.hpp>
  #include <boost/mpl/vector.hpp>
  #include <boost/mpl/size_t.hpp>

  #include <boost/preprocessor.hpp>

  namespace boost {
  namespace spirit {
  namespace prana {

  #define BOOST_PP_ITERATION_PARAMS_1                    \
    (3, (1, 8, <boost/spirit/home/prana/adt/tuple.hpp>)) \
    /***/

  template<std::size_t N, class Tuple>
  struct type_at:
    mpl::at_c<typename Tuple::type, N> { };

  template<std::size_t N, class Tuple>
  struct value_at_impl;

  #include BOOST_PP_ITERATE()
  
  template<std::size_t N, class Tuple>
  inline typename type_at<N, Tuple>::type& value_at (Tuple& t) {
    return value_at_impl<N, Tuple>()(t);
  }
  
  template<std::size_t N, class Tuple>
  inline typename type_at<N, Tuple>::type const& value_at (Tuple const& t) {
    return value_at_impl<N, Tuple const>()(t);
  }

  } // prana
  } // spirit
  } // boost

  #endif // BSP_ADT_TUPLE_HPP

#else

  #define BSP_N BOOST_PP_ITERATION()
  #define BSP_M BOOST_PP_SUB(BSP_N, 1) 

  #define BSP_DECL(z, n, data) T##n data##n;
  #define BSP_ASSIGN(z, n, data) data._##n = t##n;

  template<class Tuple>
  struct value_at_impl<BSP_M, Tuple> {
    typedef typename type_at<BSP_M, Tuple>::type& result_type;

    result_type operator() (Tuple& t) const {
      return t.BOOST_PP_CAT(_, BSP_M);
    }
  };

  template<class Tuple>
  struct value_at_impl<BSP_M, Tuple const> {
    typedef typename type_at<BSP_M, Tuple>::type const& result_type;

    result_type operator() (Tuple const& t) const {
      return t.BOOST_PP_CAT(_, BSP_M);
    }
  };

  template<BOOST_PP_ENUM_PARAMS(BSP_N, class T)>
  struct BOOST_PP_CAT(tuple, BSP_N) {
    typedef mpl::vector<BOOST_PP_ENUM_PARAMS(BSP_N, T)> type;

    typedef mpl::size_t<BSP_N> size;

    BOOST_PP_REPEAT(BSP_N, BSP_DECL, _) 
  };

  template<BOOST_PP_ENUM_PARAMS(BSP_N, class T)>
  inline BOOST_PP_CAT(tuple, BSP_N)<BOOST_PP_ENUM_PARAMS(BSP_N, T)>
  BOOST_PP_CAT(make_tuple, BSP_N)
  (BOOST_PP_ENUM_BINARY_PARAMS(BSP_N, T, const& t)) {
    BOOST_PP_CAT(tuple, BSP_N)<BOOST_PP_ENUM_PARAMS(BSP_N, T)> temp;
    BOOST_PP_REPEAT(BSP_N, BSP_ASSIGN, temp);
    return temp;
  }

  #undef BSP_N
  #undef BSP_M
  #undef BSP_DECL
  #undef BSP_ASSIGN

#endif

