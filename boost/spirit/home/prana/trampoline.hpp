/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TRAMPOLINE_HPP)
#define BOOST_SPIRIT_PRANA_TRAMPOLINE_HPP

#include <boost/config.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/order.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/deref.hpp>

#include <boost/utility/result_of.hpp>

#include <boost/preprocessor/config/limits.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/iteration/local.hpp>

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
   implementation (available ine the Boost sandbox). */

template<std::size_t Size, class Registry, class X, class Y = X>
struct trampoline;

/* EXPLAIN (wash): We do the 0 specialization here to avoid warnings. */
template<class Registry, class X, class Y>
struct trampoline<0, Registry, X, Y> {
  template<class> struct result;

  template<class This, class F, class X>
  struct result<This(F, X&)> { 
    typedef typename result_of<F(X&)>::type type;
  };  

  template<class This, class F, class X, class Y>
  struct result<This(F, X&, Y&)> {
    typedef typename result_of<F(X&, Y&)>::type type;
  }; 
  
  // EXPLAIN (djowel) Single dispatch.
  template<class F> 
  typename result<Registry::default(X&)>::type
  operator() (F f, X& x) const { 
    return Registry::default()(x);
  }

  // EXPLAIN (djowel): Double dispatch.
  template<class F>
  typename result<Registry::default(X&, Y&)>::type
  operator() (F f, X& x, Y& y) const {
    return Registry::default()(x, y);
  }
};

#define BOOST_SPIRIT_PRANA_DEF_SINGLE_DISPATCH(z, n, _)                       \
  typedef typename BOOST_PP_IF(n,                                             \
    mpl::next<BOOST_PP_CAT(iter, BOOST_PP_SUB(n, 1))>::type,                  \
    mpl::begin<Registry>::type) BOOST_PP_CAT(iter, n);                        \
                                                                              \
  typedef mpl::order<                                                         \
    Registry, typename mpl::deref<BOOST_PP_CAT(iter, n)>::type                \
  >::type BOOST_PP_CAT(hash, n):                                              \
                                                                              \
  case BOOST_PP_CAT(hash, n)::value:                                          \
    if (!Registry::fallthrough::value)                                        \
      return f(x, BOOST_PP_CAT(hash, n)::value);                              \
    else                                                                      \
      f(x, BOOST_PP_CAT(hash, n)::value);                                     \
  /***/

#define BOOST_SPIRIT_PRANA_DEF_DOUBLE_DISPATCH(z, n, _)                       \
  typedef typename BOOST_PP_IF(n,                                             \
    mpl::next<BOOST_PP_CAT(iter, BOOST_PP_SUB(n, 1))>::type,                  \
    mpl::begin<Registry>::type) BOOST_PP_CAT(iter, n);                        \
                                                                              \
  typedef mpl::order<                                                         \
    Registry, typename mpl::deref<BOOST_PP_CAT(iter, n)>::type                \
  >::type BOOST_PP_CAT(hash, n):                                              \
                                                                              \
  case BOOST_PP_CAT(hash, n)::value:                                          \
    if (!Registry::fallthrough::value)                                        \
      return f(x, BOOST_PP_CAT(hash, n)::value);                              \
    else                                                                      \
      (*this)(light_bind(f, x, BOOST_PP_CAT(hash, n)::value), y);             \
  /***/

#define BOOST_SPIRIT_PRANA_DEF_TRAMPOLINE(z, n, _)                            \
  template<class Registry, class X, class Y>                                  \
  struct trampoline<n, Registry, X, Y> {                                      \
    template<class> struct result;                                            \
                                                                              \
    template<class This, class F, class X>                                    \
    struct result<This(F, X&)> {                                              \
      typedef typename result_of<F(X&)>::type type;                           \
    };                                                                        \
                                                                              \
    template<class This, class F, class X, class Y>                           \
    struct result<This(F, X&, Y&)> {                                          \
      typedef typename result_of<F(X&, Y&)>::type type;                       \
    };                                                                        \
                                                                              \
    template<class F>                                                         \
    typename result<F(X&)>::type operator() (F f, X& x) const {               \
      switch (x.which()) {                                                    \
        BOOST_PP_REPEAT_ ## z (n, BOOST_SPIRIT_PRANA_DEF_SINGLE_DISPATCH, _)  \
        default: return Registry::default()(x);                               \
      }                                                                       \
    }                                                                         \
                                                                              \
    template<class F>                                                         \
    typename result<F(X&, Y&)>::type operator() (F f, X& x, Y& y) const {     \
      switch (x.which()) {                                                    \
        BOOST_PP_REPEAT_ ## z (n, BOOST_SPIRIT_PRANA_DEF_DOUBLE_DISPATCH, _)  \
        default: return Registry::default()(x, y);                            \
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

template<class Registry, class F, class X>
typename result_of<F(X&)>::type
dispatch (F f, X& x) {
  return trampoline<
    mpl::size<Registry>::value, Registry, X
  >()(f, x, y);
}

template<class Registry, class F, class X>
typename result_of<F(X const&)>::type
dispatch (F f, X const& x) {
  return trampoline<
    mpl::size<Registry>::value, Registry, X const
  >()(f, x, y);
}

template<class Registry, class F, class X, class Y>
typename result_of<F(X const&, Y const&)>::type
dispatch (F f, X const& x, Y const& y) {
  return trampoline<
    mpl::size<Registry>::value, Registry, X, Y
  >()(f, x, y);
}

template<class Registry, class F, class X, class Y>
typename result_of<F(X const&, Y&)>::type
dispatch (F f, X const& x, Y& y) {
  return trampoline<
    mpl::size<Registry>::value, Registry, X const, Y
  >()(f, x, y);
}

template<class Registry, class F, class X, class Y>
typename result_of<F(X&, Y const&)>::type
dispatch (F f, X x, Y const& y) {
  return trampoline<
    mpl::size<Registry>::value, Registry, X, Y const
  >()(f, x, y);
}

template<class Registry, class F, class X, class Y>
typename result_of<F(X&, Y&)>::type
dispatch (F f, X& x, Y& y) {
  return trampoline<
    mpl::size<Registry>::value, Registry, X const, Y const
  >()(f, x, y);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TRAMPOLINE_HPP

