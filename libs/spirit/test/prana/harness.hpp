/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_HARNESS_HPP)
#define BSP_HARNESS_HPP

#include <exception>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>
#include <boost/preprocessor.hpp>

#define BSP_SCHEMATIC_TEST(_, data, elem)                      \
  { std::string x = serialize(BOOST_PP_SEQ_ELEM(0, elem));                    \
    std::string y = BOOST_PP_SEQ_ELEM(1, elem);                               \
    ::boost::detail::test_eq_impl(                                            \
      x.c_str(), y.c_str(), __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, x, y  \
    ); }                                                                      \
  /***/

#define BSP_SCHEMATIC_TESTS(tests)                             \
  BOOST_PP_SEQ_FOR_EACH(BSP_SCHEMATIC_TEST, _, tests)          \
  /***/

#define BSP_STRINGIFY_TEST(_, data, elem)                      \
  { std::string x("");                                                        \
    data(BOOST_PP_SEQ_ELEM(0, elem), x);                                      \
    std::string y = BOOST_PP_SEQ_ELEM(1, elem);                               \
    ::boost::detail::test_eq_impl(                                            \
      x.c_str(), y.c_str(), __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, x, y  \
    ); }                                                                      \
  /***/

#define BSP_STRINGIFY_TESTS(func, tests)                       \
  BOOST_PP_SEQ_FOR_EACH(BSP_STRINGIFY_TEST, func, tests)       \
  /***/
  
#define BSP_BOOLEAN_TEST(_, data, elem)                    \
  BOOST_TEST_EQ(BOOST_PP_SEQ_ELEM(1, elem), BOOST_PP_SEQ_ELEM(1, elem));  \
  /***/

#define BSP_BOOLEAN_TESTS(tests)                           \
  BOOST_PP_SEQ_FOR_EACH(BSP_BOOLEAN_TEST, _, tests)        \
  /***/

#endif /*<- BSP_HARNESS_HPP ->*/

