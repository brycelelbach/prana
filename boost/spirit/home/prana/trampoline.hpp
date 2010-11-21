/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2006-2008 Steven Watanabe
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TRAMPOLINE_HPP)
#define BOOST_SPIRIT_PRANA_TRAMPOLINE_HPP

#include <boost/config.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/deref.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/config/limits.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/iteration/local.hpp>

#include <boost/spirit/home/prana/bind.hpp>

#ifndef BOOST_SPIRIT_PRANA_TRAMPOLINE_LIMIT
  #define BOOST_SPIRIT_PRANA_TRAMPOLINE_LIMIT 32
#endif

#if BOOST_SPIRIT_PRANA_TRAMPOLINE_LIMIT > BOOST_PP_LIMIT_REPEAT
  #error BOOST_SPIRIT_PRANA_TRAMPOLINE_LIMIT exceeds Boost.Preprocessor limit
#endif

#if BOOST_SPIRIT_PRANA_TRAMPOLINE_LIMIT > BOOST_PP_LIMIT_ITERATION
  #error BOOST_SPIRIT_PRANA_TRAMPOLINE_LIMIT exceeds Boost.Preprocessor limit
#endif

namespace boost {
namespace spirit {
namespace prana {

/// EXPLAIN (wash): My strategy here is based on the original visit
/// implementation for utree and Steven Watanabe's switch_ and case_
/// implementation (available in the Boost sandbox). Joel Falcou's NT2 functor
/// code was also a major source of inspiration.

template<std::size_t Size, class Registry, class F>
struct trampoline;

#define BSP_SINGLE_DISPATCH(z, n, _)                                          \
  typedef typename BOOST_PP_IF(n,                                             \
    mpl::next<BOOST_PP_CAT(iter, BOOST_PP_SUB(n, 1))>::type,                  \
    mpl::begin<Registry>::type) BOOST_PP_CAT(iter, n);                        \
                                                                              \
  typedef typename mpl::deref<BOOST_PP_CAT(iter, n)>::type                    \
    BOOST_PP_CAT(tag, n);                                                     \
                                                                              \
  case BOOST_PP_CAT(tag, n)::value:                                           \
    if (!Registry::template fallthrough<BOOST_PP_CAT(tag, n)>::value)         \
      return f(BOOST_PP_CAT(tag, n)());                                       \
    else                                                                      \
      f(BOOST_PP_CAT(tag, n)());                                              \
  /***/

#define BSP_DOUBLE_DISPATCH(z, n, _)                                          \
  typedef typename BOOST_PP_IF(n,                                             \
    mpl::next<BOOST_PP_CAT(iter, BOOST_PP_SUB(n, 1))>::type,                  \
    mpl::begin<Registry>::type) BOOST_PP_CAT(iter, n);                        \
                                                                              \
  typedef typename mpl::deref<BOOST_PP_CAT(iter, n)>::type                    \
    BOOST_PP_CAT(tag, n);                                                     \
                                                                              \
  case BOOST_PP_CAT(tag, n)::value:                                           \
    if (!Registry::template fallthrough<BOOST_PP_CAT(tag, n)>::value)         \
      return (*this)(dispatch_bind<BOOST_PP_CAT(tag, n)>(f), f);              \
    else                                                                      \
      (*this)(dispatch_bind<BOOST_PP_CAT(tag, n)>(f), f);                     \
  /***/

#define BSP_TRAMPOLINE(z, n, _)                                               \
  template<class Registry, class F>                                           \
  struct trampoline<n, Registry, F> {                                         \
    typedef F& result_type;                                                   \
                                                                              \
    F& operator() (                                                           \
      F& f, typename enable_if_c<                                             \
        F::tag_binder::value == 1, mpl::size_t<1>                             \
      >::type size = mpl::size_t<1>()                                         \
    ) const {                                                                 \
      switch (typename Registry::template which<F>()(f)) {                    \
        BOOST_PP_REPEAT_##z (n, BSP_SINGLE_DISPATCH, _)                       \
        default: return typename Registry::template default_<F>()(f);         \
      }                                                                       \
    }                                                                         \
                                                                              \
    F& operator() (                                                           \
      F& f, typename enable_if_c<                                             \
        F::tag_binder::value == 2, mpl::size_t<2>                             \
      >::type size = mpl::size_t<2>()                                         \
    ) const {                                                                 \
      switch (typename Registry::template which<F>()(f)) {                    \
        BOOST_PP_REPEAT_##z (n, BSP_DOUBLE_DISPATCH, _)                       \
        default: return typename Registry::template default_<F>()(f);         \
      }                                                                       \
    }                                                                         \
  };                                                                          \
  /***/

#define BOOST_PP_LOCAL_LIMITS (1, BOOST_SPIRIT_PRANA_TRAMPOLINE_LIMIT)
#define BOOST_PP_LOCAL_MACRO(n) BSP_TRAMPOLINE(1, n, _)

#include BOOST_PP_LOCAL_ITERATE()

#undef BSP_SINGLE_DISPATCH
#undef BSP_DOUBLE_DISPATCH
#undef BSP_TRAMPOLINE

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TRAMPOLINE_HPP

