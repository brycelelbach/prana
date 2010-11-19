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

/* EXPLAIN (wash): My strategy here is based on the original visit
   implementation for utree and Steven Watanabe's switch_ and case_
   implementation (available in the Boost sandbox). */

template<std::size_t Size, class Registry, class X, class Y = X>
struct trampoline;

/* EXPLAIN (wash): We do the 0 specialization here to avoid warnings. */
template<class Registry, class X, class Y>
struct trampoline<0, Registry, X, Y> {
  template<class> struct result;

  template<class This, class F>
  struct result<This(F, X&)> {
    typedef typename result_of<
      typename Registry::template default_<X>(X&)
    >::type type;
  };  

  template<class This, class F>
  struct result<This(F, X&, Y&)> {
    typedef typename result_of<
      typename Registry::template default_<X, Y>(X&, Y&)
    >::type type;
  };  
  
  // EXPLAIN (djowel) Single dispatch.
  template<class F> 
  typename result_of<typename Registry::template default_<X>(X&)>::type
  operator() (F f, X& x) const { 
    return typename Registry::template default_<X>()(f, x);
  }

  // EXPLAIN (djowel): Double dispatch.
  template<class F>
  typename result_of<typename Registry::template default_<X, Y>(X&, Y&)>::type
  operator() (F f, X& x, Y& y) const {
    return typename Registry::template default_<X, Y>()(f, x, y);
  }
};

#define BOOST_SPIRIT_PRANA_DEF_SINGLE_DISPATCH(z, n, _)                       \
  typedef typename BOOST_PP_IF(n,                                             \
    mpl::next<BOOST_PP_CAT(iter, BOOST_PP_SUB(n, 1))>::type,                  \
    mpl::begin<Registry>::type) BOOST_PP_CAT(iter, n);                        \
                                                                              \
  typedef typename mpl::deref<BOOST_PP_CAT(iter, n)>::type                    \
    BOOST_PP_CAT(tag, n);                                                     \
                                                                              \
  case BOOST_PP_CAT(tag, n)::value:                                           \
    if (!Registry::template fallthrough<BOOST_PP_CAT(tag, n)>::value)         \
      return f(x);                                                            \
    else                                                                      \
      f(x);                                                                   \
  /***/

#define BOOST_SPIRIT_PRANA_DEF_DOUBLE_DISPATCH(z, n, _)                       \
  typedef typename BOOST_PP_IF(n,                                             \
    mpl::next<BOOST_PP_CAT(iter, BOOST_PP_SUB(n, 1))>::type,                  \
    mpl::begin<Registry>::type) BOOST_PP_CAT(iter, n);                        \
                                                                              \
  typedef typename mpl::deref<BOOST_PP_CAT(iter, n)>::type                    \
    BOOST_PP_CAT(tag, n);                                                     \
                                                                              \
  case BOOST_PP_CAT(tag, n)::value:                                           \
    if (!Registry::template fallthrough<BOOST_PP_CAT(tag, n)>::value)         \
      return (*this)(lhs_bind(f, x), y);                                      \
    else                                                                      \
      (*this)(lhs_bind(f, x), y);                                             \
  /***/

#define BOOST_SPIRIT_PRANA_DEF_TRAMPOLINE(z, n, _)                            \
  template<class Registry, class X, class Y>                                  \
  struct trampoline<n, Registry, X, Y> {                                      \
    template<class> struct result;                                            \
                                                                              \
    template<class This, class F>                                             \
    struct result<This(F, X&)> {                                              \
      typedef typename F::result_type type;                                   \
    };                                                                        \
                                                                              \
    template<class This, class F>                                             \
    struct result<This(F, X&, Y&)> {                                          \
      typedef typename F::result_type type;                                   \
    };                                                                        \
                                                                              \
    template<class F>                                                         \
    typename F::result_type operator() (F f, X& x) const {                    \
      switch (typename Registry::template which<X>()(x)) {                    \
        BOOST_PP_REPEAT_##z (n, BOOST_SPIRIT_PRANA_DEF_SINGLE_DISPATCH, _)    \
        default:                                                              \
          return typename Registry::template default_<X>()(f, x);             \
      }                                                                       \
    }                                                                         \
                                                                              \
    template<class F>                                                         \
    typename F::result_type operator() (F f, X& x, Y& y) const {              \
      switch (typename Registry::template which<X>()(x)) {                    \
        BOOST_PP_REPEAT_##z (n, BOOST_SPIRIT_PRANA_DEF_DOUBLE_DISPATCH, _)    \
        default:                                                              \
          return typename Registry::template default_<X, Y>()(f, x, y);       \
      }                                                                       \
    }                                                                         \
  };                                                                          \
  /***/

#define BOOST_PP_LOCAL_LIMITS (1, BOOST_SPIRIT_PRANA_TRAMPOLINE_LIMIT)
#define BOOST_PP_LOCAL_MACRO(n) BOOST_SPIRIT_PRANA_DEF_TRAMPOLINE(1, n, _)

#include BOOST_PP_LOCAL_ITERATE()

#undef BOOST_SPIRIT_PRANA_DEF_TRAMPOLINE
#undef BOOST_SPIRIT_PRANA_DEF_SINGLE_DISPATCH
#undef BOOST_SPIRIT_PRANA_DEF_DOUBLE_DISPATCH

/* EXPLAIN (wash): Single dispatch. */

template<class Registry, class F, class X>
typename result_of<
  trampoline<
    mpl::size<Registry>::value, Registry, X, X
  >(F, X&)
>::type dispatch (F f, X& x) {
  return trampoline<
    mpl::size<Registry>::value, Registry, X, X
  >()(f, x);
}

template<class Registry, class F, class X>
typename result_of<
  trampoline<
    mpl::size<Registry>::value, Registry, X const, X const
  >(F, X const&)
>::type dispatch (F f, X const& x) {
  return trampoline<
    mpl::size<Registry>::value, Registry, X const, X const
  >()(f, x);
}

/* EXPLAIN (wash): Double dispatch. */

template<class Registry, class F, class X, class Y>
typename result_of<
  trampoline<
    mpl::size<Registry>::value, Registry, X const, Y const
  >(F, X const&, Y const&)
>::type dispatch (F f, X const& x, Y const& y) {
  return trampoline<
    mpl::size<Registry>::value, Registry, X const, Y const
  >()(f, x, y);
}

template<class Registry, class F, class X, class Y>
typename result_of<
  trampoline<
    mpl::size<Registry>::value, Registry, X const, Y 
  >(F, X const&, Y&)
>::type dispatch (F f, X const& x, Y& y) {
  return trampoline<
    mpl::size<Registry>::value, Registry, X const, Y
  >()(f, x, y);
}

template<class Registry, class F, class X, class Y>
typename result_of<
  trampoline<
    mpl::size<Registry>::value, Registry, X, Y const
  >(F, X&, Y const&)
>::type dispatch (F f, X x, Y const& y) {
  return trampoline<
    mpl::size<Registry>::value, Registry, X, Y const
  >()(f, x, y);
}

template<class Registry, class F, class X, class Y>
typename result_of<
  trampoline<
    mpl::size<Registry>::value, Registry, X, Y
  >(F, X&, Y&)
>::type dispatch (F f, X& x, Y& y) {
  return trampoline<
    mpl::size<Registry>::value, Registry, X, Y 
  >()(f, x, y);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TRAMPOLINE_HPP

