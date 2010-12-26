/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2006-2008 Steven Watanabe
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_DISPATCH_TRAMPOLINE_HPP)
#define BOOST_SPIRIT_PRANA_DISPATCH_TRAMPOLINE_HPP

#include <boost/config.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/deref.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/config/limits.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/iteration/local.hpp>

#include <boost/spirit/home/prana/dispatch/bind.hpp>
#include <boost/spirit/home/prana/domain.hpp>

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

//[trampoline

// My strategy here is based on the original visit implementation for utree and
// Steven Watanabe's switch_ and case_ mplementation (available in the Boost
// sandbox). Joel Falcou's NT2 functor code was also a major source of
// inspiration.

template<std::size_t Size, class Registry, class Result, class F,
         class Dummy = void>
struct trampoline;
//]

#define BSP_SINGLE_DISPATCH(z, m, n)                                          \
  typedef typename BOOST_PP_IF(m,                                             \
    mpl::next<BOOST_PP_CAT(iter, BOOST_PP_SUB(m, 1))>::type,                  \
    mpl::begin<Registry>::type) BOOST_PP_CAT(iter, m);                        \
                                                                              \
  typedef typename mpl::deref<BOOST_PP_CAT(iter, m)>::type                    \
    BOOST_PP_CAT(tag, m);                                                     \
                                                                              \
  case BOOST_PP_CAT(tag, m)::value:                                           \
    return f(BOOST_PP_CAT(tag, m)());                                         \
  /***/

#define BSP_DOUBLE_DISPATCH(z, m, n)                                          \
  typedef typename BOOST_PP_IF(m,                                             \
    mpl::next<BOOST_PP_CAT(iter, BOOST_PP_SUB(m, 1))>::type,                  \
    mpl::begin<Registry>::type) BOOST_PP_CAT(iter, m);                        \
                                                                              \
  typedef typename mpl::deref<BOOST_PP_CAT(iter, m)>::type                    \
    BOOST_PP_CAT(tag, m);                                                     \
                                                                              \
  case BOOST_PP_CAT(tag, m)::value: {                                         \
    dispatch_binder<BOOST_PP_CAT(tag, m), F> fd(f);                           \
    return trampoline<                                                        \
      n, Registry, Result, dispatch_binder<BOOST_PP_CAT(tag, m), F>,          \
      mpl::size_t<1>                                                          \
    >()(fd);                                                                  \
  }                                                                           \
  /***/

#define BSP_TRAMPOLINE(z, n, _)                                               \
  template<class Registry, class Result, class F>                             \
  struct trampoline<                                                          \
    n, Registry, Result, F,                                                   \
    typename enable_if_c<                                                     \
      F::tag_binder::value == 1,                                              \
      mpl::size_t<1>                                                          \
    >::type                                                                   \
  > {                                                                         \
    typedef Result result_type;                                               \
                                                                              \
    result_type operator() (F& f) const {                                     \
      switch (typename Registry::which()(f)) {                                \
        BOOST_PP_REPEAT_##z (n, BSP_SINGLE_DISPATCH, n)                       \
        default: return typename Registry::template default_<Result>()(f);    \
      }                                                                       \
    }                                                                         \
  };                                                                          \
                                                                              \
  template<class Registry, class Result, class F>                             \
  struct trampoline<                                                          \
    n, Registry, Result, F,                                                   \
    typename enable_if_c<                                                     \
      F::tag_binder::value == 2,                                              \
      mpl::size_t<2>                                                          \
    >::type                                                                   \
  > {                                                                         \
    typedef Result result_type;                                               \
                                                                              \
    result_type operator() (F& f) const {                                     \
      switch (typename Registry::which()(f)) {                                \
        BOOST_PP_REPEAT_##z (n, BSP_DOUBLE_DISPATCH, n)                       \
        default: return typename Registry::template default_<Result>()(f);    \
      }                                                                       \
    }                                                                         \
                                                                              \
    template<class Tag>                                                       \
    result_type operator() (F& f, Tag) const {                                \
      dispatch_binder<Tag, F> fd(f);                                          \
      return trampoline<                                                      \
        n, Registry, Result, dispatch_binder<Tag, F>, mpl::size_t<1>          \
      >()(fd);                                                                \
    }                                                                         \
  };                                                                          \
  /***/

#define BOOST_PP_LOCAL_LIMITS (1, BOOST_SPIRIT_PRANA_TRAMPOLINE_LIMIT)
#define BOOST_PP_LOCAL_MACRO(n) BSP_TRAMPOLINE(1, n, _)

#include BOOST_PP_LOCAL_ITERATE()
  
#define BSP_PRE(r, data, elem)    data elem 
#define BSP_POST(r, data, elem)   elem data
#define BSP_FIRST(r, data, elem)  BOOST_PP_SEQ_HEAD(elem)

#define BSP_EMPTY_PARAMS(x, y)  
#define BSP_EMPTY_BINARY_PARAMS(x, y, z)

#define BSP_ARGS(r, data, i, elem)                               \
  BOOST_PP_COMMA_IF(i)                                           \
  BOOST_PP_SEQ_FOR_EACH_R(r, BSP_POST, BOOST_PP_EMPTY(), elem) & \
  BOOST_PP_CAT(data, i)                                          \
  /***/

#define BSP_ALL(r, data, elem)                                   \
  BOOST_PP_SEQ_FOR_EACH_R(r, BSP_POST, BOOST_PP_EMPTY(), elem)   \
  /***/

#define BSP_FN(name, actors, num_tags)                                      \
  template<                                                                 \
    class Registry, template<                                               \
      BOOST_PP_ENUM_PARAMS(                                                 \
        BOOST_PP_ADD(num_tags, 1), class BOOST_PP_INTERCEPT)                \
    > class F,                                                              \
    BOOST_PP_IIF(BOOST_PP_LESS(BOOST_PP_SEQ_SIZE(actors), num_tags),        \
      BOOST_PP_ENUM_PARAMS, BSP_EMPTY_PARAMS)                               \
      (BOOST_PP_SUB(num_tags, BOOST_PP_SEQ_SIZE(actors)), class Tag)        \
    BOOST_PP_COMMA_IF(BOOST_PP_LESS(BOOST_PP_SEQ_SIZE(actors), num_tags))   \
    BOOST_PP_SEQ_ENUM(                                                      \
      BOOST_PP_SEQ_TRANSFORM(BSP_PRE, class,                                \
        BOOST_PP_SEQ_TRANSFORM(BSP_FIRST, _, actors)))                      \
  >                                                                         \
  inline typename boost::result_of<F<                                       \
    BOOST_PP_ENUM_PARAMS(                                                   \
      num_tags, prana::unused_type BOOST_PP_INTERCEPT), void                \
  >(                                                                        \
    BOOST_PP_SEQ_ENUM(                                                      \
      BOOST_PP_SEQ_TRANSFORM(BSP_POST, &,                                   \
        BOOST_PP_SEQ_TRANSFORM(BSP_ALL, _, actors)))                        \
  )>::type name (                                                           \
    BOOST_PP_SEQ_FOR_EACH_I(BSP_ARGS, a, actors)                            \
  ) {                                                                       \
    BOOST_PP_CAT(bind_,                                                     \
      BOOST_PP_CAT(BOOST_PP_SEQ_SIZE(actors),                               \
        BOOST_PP_CAT(x,                                                     \
          BOOST_PP_CAT(num_tags, _tag_fn))))<                               \
      F, BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(BSP_ALL, _, actors))      \
    > fd(bind_tag_fn<F>(                                                    \
      BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(actors), a)                    \
    ));                                                                     \
    return trampoline<                                                      \
      mpl::size<Registry>::value, Registry,                                 \
      typename boost::result_of<F<                                          \
        BOOST_PP_ENUM_PARAMS(                                               \
          num_tags, prana::unused_type BOOST_PP_INTERCEPT), void            \
      >(                                                                    \
        BOOST_PP_SEQ_ENUM(                                                  \
          BOOST_PP_SEQ_TRANSFORM(BSP_POST, &,                               \
            BOOST_PP_SEQ_TRANSFORM(BSP_ALL, _, actors)))                    \
      )>::type,                                                             \
      BOOST_PP_CAT(bind_,                                                   \
        BOOST_PP_CAT(BOOST_PP_SEQ_SIZE(actors),                             \
          BOOST_PP_CAT(x,                                                   \
            BOOST_PP_CAT(num_tags, _tag_fn))))<                             \
        F, BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(BSP_ALL, _, actors))    \
      >,                                                                    \
      mpl::size_t<num_tags>                                                 \
    >()(fd                                                                  \
      BOOST_PP_COMMA_IF(BOOST_PP_LESS(BOOST_PP_SEQ_SIZE(actors), num_tags)) \
      BOOST_PP_IIF(BOOST_PP_LESS(BOOST_PP_SEQ_SIZE(actors), num_tags),      \
        BOOST_PP_ENUM_BINARY_PARAMS, BSP_EMPTY_BINARY_PARAMS)               \
      (BOOST_PP_SUB(num_tags, BOOST_PP_SEQ_SIZE(actors)),                   \
        Tag, () BOOST_PP_INTERCEPT)                                         \
    );                                                                      \
  }                                                                         \
  /***/

BSP_FN(dispatch, ((A0)),        1);
BSP_FN(dispatch, ((A0)(const)), 1);

BSP_FN(dispatch, ((A0)),        2);
BSP_FN(dispatch, ((A0)(const)), 2);

BSP_FN(dispatch, ((A0))        ((A1)),        1);
BSP_FN(dispatch, ((A0)(const)) ((A1)),        1);
BSP_FN(dispatch, ((A0))        ((A1)(const)), 1);
BSP_FN(dispatch, ((A0)(const)) ((A1)(const)), 1);

BSP_FN(dispatch, ((A0))        ((A1)),        2);
BSP_FN(dispatch, ((A0)(const)) ((A1)),        2);
BSP_FN(dispatch, ((A0))        ((A1)(const)), 2);
BSP_FN(dispatch, ((A0)(const)) ((A1)(const)), 2);

#undef BSP_SINGLE_DISPATCH
#undef BSP_DOUBLE_DISPATCH
#undef BSP_TRAMPOLINE
#undef BSP_PRE
#undef BSP_POST
#undef BSP_FIRST
#undef BSP_ARGS
#undef BSP_FN

} // prana 
} // spirit 
} // boost 

#endif // BOOST_SPIRIT_PRANA_DISPATCH_TRAMPOLINE_HPP

