/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_BIND_HPP)
#define BOOST_SPIRIT_PRANA_BIND_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/mpl/size_t.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/container/vector/vector_fwd.hpp>
#include <boost/fusion/include/vector_fwd.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor.hpp>

#include <boost/spirit/home/prana/domain.hpp>

namespace boost {
namespace spirit {
namespace prana {

#define BSP_LEFTMOST  0
#define BSP_RIGHTMOST 1

#define BSP_PRE(r, data, elem)                          \
  data elem                                             \
  /***/

#define BSP_POST(r, data, elem)                         \
  elem data                                             \
  /***/

#define BSP_DECL(r, data, elem)                         \
  elem& BOOST_PP_CAT(data, elem);                       \
  /***/

#define BSP_PARAMS(r, data, elem)                       \
  elem& BOOST_PP_CAT(elem, data)                        \
  /***/

#define BSP_ARGS(r, data, i, elem)                               \
  BOOST_PP_COMMA_IF(i)                                           \
  BOOST_PP_SEQ_FOR_EACH_R(r, BSP_POST, BOOST_PP_EMPTY(), elem) & \
  BOOST_PP_CAT(data, i)                                          \
  /***/

#define BSP_ALL(r, data, elem)                                   \
  BOOST_PP_SEQ_FOR_EACH_R(r, BSP_POST, BOOST_PP_EMPTY(), elem)   \
  /***/

#define BSP_FIRST(r, data, elem)                        \
  BOOST_PP_SEQ_HEAD(elem)                               \
  /***/

#define BSP_INIT(r, data, i, elem)                      \
  BOOST_PP_COMMA_IF(i) BOOST_PP_CAT(elem, data)         \
  /***/

#define BSP_CALL(r, data, i, elem)                      \
  BOOST_PP_COMMA_IF(i)                                  \
  fusion::at_c<i>(value)                                \
  /***/

#define BSP_EMPTY(x, y) \
  /***/

#define BSP_GET(r, data, i, elem)                                         \
  template<std::size_t N>                                                 \
  typename enable_if_c<N == i, elem&>::type get (void) {                  \
    return fusion::at_c<N>(value);                                        \
  }                                                                       \
                                                                          \
  template<std::size_t N>                                                 \
  typename enable_if_c<N == i, elem const&>::type get (void) const {      \
    return fusion::at_c<N>(value);                                        \
  }                                                                       \
  /***/

#define BSP_BINDER(name, actors, num_tags)                                  \
  template<                                                                 \
    template<BOOST_PP_ENUM_PARAMS(                                          \
      BOOST_PP_ADD(num_tags, 1), class BOOST_PP_INTERCEPT)> class F,        \
    BOOST_PP_SEQ_ENUM(                                                      \
      BOOST_PP_SEQ_TRANSFORM(BSP_PRE, class,                                \
        BOOST_PP_SEQ_TRANSFORM(BSP_FIRST, _, actors)))                      \
  >                                                                         \
  class name {                                                              \
   public:                                                                  \
    typedef fusion::BOOST_PP_CAT(vector, BOOST_PP_SEQ_SIZE(actors))<        \
      BOOST_PP_SEQ_ENUM(                                                    \
        BOOST_PP_SEQ_TRANSFORM(BSP_POST, &,                                 \
          BOOST_PP_SEQ_TRANSFORM(BSP_FIRST, _, actors)))                    \
    > value_type;                                                           \
                                                                            \
   private:                                                                 \
    value_type value;                                                       \
                                                                            \
   public:                                                                  \
    typedef mpl::size_t<num_tags>::type tag_binder;                         \
                                                                            \
    BOOST_PP_SEQ_FOR_EACH_I(BSP_GET, _,                                     \
      BOOST_PP_SEQ_TRANSFORM(BSP_FIRST, _, actors))                         \
                                                                            \
    template<class> struct result;                                          \
                                                                            \
    template<                                                               \
      class This,                                                           \
      BOOST_PP_ENUM_PARAMS(num_tags, class Tag)                             \
    >                                                                       \
    struct result<                                                          \
      This(                                                                 \
        BOOST_PP_ENUM_BINARY_PARAMS(num_tags,                               \
          Tag, const& BOOST_PP_INTERCEPT)                                   \
      )                                                                     \
    > {                                                                     \
      typedef typename result_of<                                           \
        F<                                                                  \
          BOOST_PP_ENUM_BINARY_PARAMS(num_tags,                             \
            Tag, const& BOOST_PP_INTERCEPT),                                \
            prana::unused_type                                              \
        >(                                                                  \
          BOOST_PP_SEQ_ENUM(                                                \
            BOOST_PP_SEQ_TRANSFORM(BSP_POST, &,                             \
              BOOST_PP_SEQ_TRANSFORM(BSP_FIRST, _, actors)))                \
        )                                                                   \
      >::type type;                                                         \
    };                                                                      \
                                                                            \
    name (BOOST_PP_SEQ_ENUM(                                                \
      BOOST_PP_SEQ_TRANSFORM(BSP_PARAMS, _,                                 \
        BOOST_PP_SEQ_TRANSFORM(BSP_FIRST, _, actors)))):                    \
          value(BOOST_PP_SEQ_FOR_EACH_I(BSP_INIT, _,                        \
            BOOST_PP_SEQ_TRANSFORM(BSP_FIRST, _, actors))) { }              \
                                                                            \
    template<BOOST_PP_ENUM_PARAMS(num_tags, class Tag)>                     \
    typename result_of<name(                                                \
      BOOST_PP_ENUM_BINARY_PARAMS(num_tags, Tag, const& BOOST_PP_INTERCEPT) \
    )>::type operator() (                                                   \
      BOOST_PP_ENUM_BINARY_PARAMS(num_tags, Tag, const& BOOST_PP_INTERCEPT) \
    ) const {                                                               \
      return F<                                                             \
        BOOST_PP_ENUM_BINARY_PARAMS(num_tags,                               \
          Tag, const& BOOST_PP_INTERCEPT),                                  \
          prana::unused_type                                                \
      >()(                                                                  \
        BOOST_PP_SEQ_FOR_EACH_I(BSP_CALL, _,                                \
          BOOST_PP_SEQ_TRANSFORM(BSP_FIRST, _, actors))                     \
      );                                                                    \
    }                                                                       \
  };                                                                        \
  /***/

#define BSP_FN(name, actors, num_tags, binder)                              \
  template<                                                                 \
    template<BOOST_PP_ENUM_PARAMS(                                          \
      BOOST_PP_ADD(num_tags, 1), class BOOST_PP_INTERCEPT)> class F,        \
    BOOST_PP_SEQ_ENUM(                                                      \
      BOOST_PP_SEQ_TRANSFORM(BSP_PRE, class,                                \
        BOOST_PP_SEQ_TRANSFORM(BSP_FIRST, _, actors)))                      \
  >                                                                         \
  inline binder<                                                            \
    F, BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(BSP_ALL, _, actors)         \
  )> name (                                                                 \
    BOOST_PP_SEQ_FOR_EACH_I(BSP_ARGS, a, actors)                            \
  ) {                                                                       \
    return binder<                                                          \
      F, BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(BSP_ALL, _, actors)       \
    )>(                                                                     \
      BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(actors), a)                    \
    );                                                                      \
  }                                                                         \
  /***/

BSP_BINDER(bind_1x1_tag_fn, ((A0)),       1)

BSP_BINDER(bind_1x2_tag_fn, ((A0)),       2)

BSP_BINDER(bind_2x1_tag_fn, ((A0))((A1)), 1)

BSP_BINDER(bind_2x2_tag_fn, ((A0))((A1)), 2)

BSP_FN(bind_tag_fn, ((A0)),        1, bind_1x1_tag_fn)
BSP_FN(bind_tag_fn, ((A0)(const)), 1, bind_1x1_tag_fn)

BSP_FN(bind_tag_fn, ((A0)),        2, bind_1x2_tag_fn)
BSP_FN(bind_tag_fn, ((A0)(const)), 2, bind_1x2_tag_fn)

BSP_FN(bind_tag_fn, ((A0))        ((A1)),        1, bind_2x1_tag_fn)
BSP_FN(bind_tag_fn, ((A0)(const)) ((A1)),        1, bind_2x1_tag_fn)
BSP_FN(bind_tag_fn, ((A0))        ((A1)(const)), 1, bind_2x1_tag_fn)
BSP_FN(bind_tag_fn, ((A0)(const)) ((A1)(const)), 1, bind_2x1_tag_fn)

BSP_FN(bind_tag_fn, ((A0))        ((A1)),        2, bind_2x2_tag_fn)
BSP_FN(bind_tag_fn, ((A0)(const)) ((A1)),        2, bind_2x2_tag_fn)
BSP_FN(bind_tag_fn, ((A0))        ((A1)(const)), 2, bind_2x2_tag_fn)
BSP_FN(bind_tag_fn, ((A0)(const)) ((A1)(const)), 2, bind_2x2_tag_fn)

/// EXPLAIN (djowel): Simple binder for binary visitation (we don't want to
/// bring in the big guns). 
template<class TagX, class F, class Dummy = prana::unused_type>
class dispatch_binder {
 private:
  F& f; /// EXPLAIN (wash): We must take a reference to the tag binder.  

 public:
  typedef mpl::size_t<1>::type tag_binder;

  template<class> struct result;

  template<class This, class TagY>
  struct result<This(TagY const&)> {
    typedef typename result_of<F(TagX const&, TagY const&)>::type type;
  };
  
  dispatch_binder (F& f_): f(f_) { }

  template<class TagY>
  typename result_of<F(TagX const&, TagY const&)>::type
  operator() (TagY const& tagy) const {
    return f(TagX(), tagy);
  }
};

template<class TagX, class F>
dispatch_binder<TagX, F> dispatch_bind (F& f) {
  return dispatch_binder<TagX, F>(f);
}

template<class TagX, class F>
dispatch_binder<TagX, F const> dispatch_bind (F const& f) {
  return dispatch_binder<TagX, F const>(f);
}

#undef BSP_LEFTMOST
#undef BSP_RIGHTMOST
#undef BSP_PRE
#undef BSP_POST
#undef BSP_DECL
#undef BSP_PARAMS
#undef BSP_ARGS
#undef BSP_ALL
#undef BSP_FIRST
#undef BSP_INIT 
#undef BSP_CALL
#undef BSP_GET
#undef BSP_EMPTY
#undef BSP_BINDER
#undef BSP_FN

} /// prana
} /// spirit
} /// boost

#endif /// BOOST_SPIRIT_PRANA_BIND_HPP


