/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_HARNESS_HPP)
#define BOOST_SPIRIT_PRANA_HARNESS_HPP

#include <sstream>

#include <boost/detail/lightweight_test.hpp>
#include <boost/preprocessor.hpp>

#define BOOST_SPIRIT_PRANA_SEXPR_TEST(_, data, elem)                          \
  { std::string x = serialize(BOOST_PP_SEQ_ELEM(0, elem));                    \
    std::string y = BOOST_PP_SEQ_ELEM(1, elem);                               \
    ::boost::detail::test_eq_impl(                                            \
      x.c_str(), y.c_str(), __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, x, y  \
    ); }                                                                      \
  /***/

#define BOOST_SPIRIT_PRANA_SEXPR_TESTS(tests)                             \
  BOOST_PP_SEQ_FOR_EACH(BOOST_SPIRIT_PRANA_SEXPR_TEST, _, tests)          \
  /***/

#define BOOST_SPIRIT_PRANA_UTREE_TEST(_, data, elem)                          \
  { std::string x("");                                                        \
    data(BOOST_PP_SEQ_ELEM(0, elem), x);                                      \
    std::string y = BOOST_PP_SEQ_ELEM(1, elem);                               \
    ::boost::detail::test_eq_impl(                                            \
      x.c_str(), y.c_str(), __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, x, y  \
    ); }                                                                      \
  /***/

#define BOOST_SPIRIT_PRANA_UTREE_TESTS(func, tests)                       \
  BOOST_PP_SEQ_FOR_EACH(BOOST_SPIRIT_PRANA_UTREE_TEST, func, tests)       \
  /***/
  
#define BOOST_SPIRIT_PRANA_BOOLEAN_TEST(_, data, elem)                    \
  BOOST_TEST_EQ(BOOST_PP_SEQ_ELEM(1, elem), BOOST_PP_SEQ_ELEM(1, elem));  \
  /***/

#define BOOST_SPIRIT_PRANA_BOOLEAN_TESTS(tests)                           \
  BOOST_PP_SEQ_FOR_EACH(BOOST_SPIRIT_PRANA_BOOLEAN_TEST, _, tests)        \
  /***/

#endif /*<- BOOST_SPIRIT_PRANA_HARNESS_HPP ->*/

