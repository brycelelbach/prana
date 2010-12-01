/*<-============================================================================
    Copyright (c) 2010 Joel Falcou
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SUPPORT_ENABLE_IF_TYPE_HPP)
#define BOOST_SPIRIT_PRANA_SUPPORT_ENABLE_IF_TYPE_HPP

#include <boost/preprocessor.hpp>

#include <boost/spirit/home/prana/enable_if_type.hpp>
#include <boost/spirit/home/prana/domain.hpp>

namespace boost {
namespace spirit {
namespace prana {

#define BOOST_SPIRIT_PRANA_SFINAE_TEST_FOR(r, test, elem)                 \
  template<class T>                                                       \
  struct test<T, typename enable_if_type<typename T::elem>::type>: T { }; \
  /***/

#define BOOST_SPIRIT_PRANA_SFINAE_TEST(test, members)                       \
  template<class T, class Dumy = prana::unused_type>                        \
  struct test { typedef prana::unused_type type; };                         \
                                                                            \
  BOOST_PP_SEQ_FOR_EACH(BOOST_SPIRIT_PRANA_SFINAE_TEST_FOR, test, members)  \
  /***/

template<class T, class Dummy = prana::unused_type>
struct enable_if_type;

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SUPPORT_ENABLE_IF_TYPE_HPP ->*/
