/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !BOOST_PP_IS_ITERATING
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
  #include <boost/preprocessor/seq.hpp>

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

  #define BSP_INIT(r, data, elem)                         \
    BOOST_PP_CAT(data, elem) ( BOOST_PP_CAT(elem, data) ) \
    /***/

  #define BSP_CALL(r, data, i, elem)                      \
    BOOST_PP_COMMA_IF(i)                                  \
    fusion::at_c<i>(value)                                \
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
 
  #define BSP_BINDER(name, actors, num_tags, bound_side)                      \
    template<                                                                 \
      BOOST_PP_ENUM_PARAMS(BOOST_PP_SUB(num_tags, 1), class Tag),             \
      template<class, class> class F,                                         \
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
      typedef mpl::size_t<BOOST_PP_SEQ_SIZE(actors)>::type tag_binder;        \
                                                                              \
      BOOST_PP_SEQ_FOR_EACH_I(BSP_GET, _,                                     \
        BOOST_PP_SEQ_TRANSFORM(BSP_FIRST, _, actors))                         \
                                                                              \
      template<class> struct result;                                          \
                                                                              \
      template<                                                               \
        class This, class                                                     \
        BOOST_PP_IIF(BOOST_PP_BOOL(bound_side), Leftmost, Rightmost)          \
      >                                                                       \
      struct result<                                                          \
        This(BOOST_PP_IIF(BOOST_PP_BOOL(bound_side), Leftmost, Rightmost))    \
      > {                                                                     \
        typedef typename result_of<                                           \
          F<                                                                  \
            BOOST_PP_IIF(BOOST_PP_BOOL(bound_side),                           \
              Leftmost BOOST_PP_COMMA, BOOST_PP_EMPTY)()                      \
            BOOST_PP_ENUM_PARAMS(BOOST_PP_SUB(num_tags, 1), Tag)              \
            BOOST_PP_IIF(BOOST_PP_BOOL(bound_side),                           \
              BOOST_PP_EMPTY, BOOST_PP_COMMA)()                               \
            BOOST_PP_IIF(BOOST_PP_BOOL(bound_side),                           \
              BOOST_PP_EMPTY(), Rightmost)                                    \
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
            value(BOOST_PP_SEQ_ENUM(                                          \
              BOOST_PP_SEQ_TRANSFORM(BSP_INIT, _,                             \
                BOOST_PP_SEQ_TRANSFORM(BSP_FIRST, _, actors)))) { }           \
                                                                              \
      template<class BOOST_PP_IIF(                                            \
        BOOST_PP_BOOL(bound_side), Leftmost, Rightmost)>                      \
      typename result_of<name(BOOST_PP_IIF(                                   \
        BOOST_PP_BOOL(bound_side), Leftmost, Rightmost)                       \
      )>::type operator() (                                                   \
        BOOST_PP_IIF(BOOST_PP_BOOL(bound_side), Leftmost, Rightmost)          \
      ) const {                                                               \
        return F<                                                             \
            BOOST_PP_IIF(BOOST_PP_BOOL(bound_side),                           \
              Leftmost BOOST_PP_COMMA, BOOST_PP_EMPTY)()                      \
            BOOST_PP_ENUM_PARAMS(BOOST_PP_SUB(num_tags, 1), Tag)              \
            BOOST_PP_IIF(BOOST_PP_BOOL(bound_side),                           \
              BOOST_PP_EMPTY, BOOST_PP_COMMA)()                               \
            BOOST_PP_IIF(BOOST_PP_BOOL(bound_side),                           \
              BOOST_PP_EMPTY(), Rightmost)                                    \
          >()(                                                                \
            BOOST_PP_SEQ_FOR_EACH_I(BSP_CALL, _,                              \
              BOOST_PP_SEQ_TRANSFORM(BSP_FIRST, _, actors))                   \
          );                                                                  \
      }                                                                       \
    };                                                                        \
    /***/

  #define BSP_FN(name, actors, binder)                                        \
    template<                                                                 \
      class Tag, template<class, class> class F,                              \
      BOOST_PP_SEQ_ENUM(                                                      \
        BOOST_PP_SEQ_TRANSFORM(BSP_PRE, class,                                \
          BOOST_PP_SEQ_TRANSFORM(BSP_FIRST, _, actors)))                      \
    >                                                                         \
    inline binder<Tag, F, BOOST_PP_SEQ_ENUM(                                  \
      BOOST_PP_SEQ_TRANSFORM(BSP_ALL, _, actors)                              \
    )> name (                                                                 \
      BOOST_PP_SEQ_FOR_EACH_I(BSP_ARGS, a, actors)                            \
    ) {                                                                       \
      return binder<Tag, F, BOOST_PP_SEQ_ENUM(                                \
        BOOST_PP_SEQ_TRANSFORM(BSP_ALL, _, actors)                            \
      )>(                                                                     \
        BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(actors), a)                    \
      );                                                                      \
    }                                                                         \
    /***/

  #define BOOST_PP_ITERATION_PARAMS_1               \
    (3, (1, 5, <boost/spirit/home/prana/bind.hpp>)) \
    /***/

  #include BOOST_PP_ITERATE()

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
  #undef BSP_BINDER
  #undef BSP_FN

  } /// prana
  } /// spirit
  } /// boost

  #endif /// BOOST_SPIRIT_PRANA_BIND_HPP

#elif BOOST_PP_ITERATION() == 1
  BSP_BINDER(lhs_unary_tag_binder, ((A0)), 2, BSP_LEFTMOST)
  
  BSP_FN(bind_lhs_tag, ((A0)),        lhs_unary_tag_binder)
  BSP_FN(bind_lhs_tag, ((A0)(const)), lhs_unary_tag_binder)

#elif BOOST_PP_ITERATION() == 2
  BSP_BINDER(rhs_unary_tag_binder, ((A0)), 2, BSP_RIGHTMOST)
  
  BSP_FN(bind_rhs_tag, ((A0)),        rhs_unary_tag_binder)
  BSP_FN(bind_rhs_tag, ((A0)(const)), rhs_unary_tag_binder)

#elif BOOST_PP_ITERATION() == 3
  BSP_BINDER(lhs_binary_tag_binder, ((A0))((A1)), 2, BSP_LEFTMOST)

  BSP_FN(bind_lhs_tag, ((A0))        ((A1)),        lhs_binary_tag_binder)
  BSP_FN(bind_lhs_tag, ((A0)(const)) ((A1)),        lhs_binary_tag_binder)
  BSP_FN(bind_lhs_tag, ((A0))        ((A1)(const)), lhs_binary_tag_binder)
  BSP_FN(bind_lhs_tag, ((A0)(const)) ((A1)(const)), lhs_binary_tag_binder)
  
#elif BOOST_PP_ITERATION() == 4
  BSP_BINDER(rhs_binary_tag_binder, ((A0))((A1)), 2, BSP_RIGHTMOST)
  
  BSP_FN(bind_rhs_tag, ((A0))        ((A1)),        rhs_binary_tag_binder)
  BSP_FN(bind_rhs_tag, ((A0)(const)) ((A1)),        rhs_binary_tag_binder)
  BSP_FN(bind_rhs_tag, ((A0))        ((A1)(const)), rhs_binary_tag_binder)
  BSP_FN(bind_rhs_tag, ((A0)(const)) ((A1)(const)), rhs_binary_tag_binder)

#elif BOOST_PP_ITERATION() == 5
  /// EXPLAIN (djowel): Simple binder for binary visitation (we don't want to
  /// bring in the big guns). 
  template<class TagX, class F>
  class dispatch_binder {
   private:
    F& f; /// EXPLAIN (wash): We must take a reference to the tag binder.  

   public:
    template<class> struct result;

    template<class This, class TagY>
    struct result<This(TagY)> {
      typedef typename result_of<F(TagX, TagY)>::type type;
    };
    
    dispatch_binder (F& f_): f(f_) { }

    template<class TagY>
    typename result_of<F(TagX, TagY)>::type operator() (TagY tagy) const {
      return f(TagX(), tagy);
    }
  };

  template<class TagX, class F>
  dispatch_binder<TagX, F> dispatch_bind(F& f) {
    return dispatch_binder<TagX, F>(f);
  }
  
  template<class TagX, class F>
  dispatch_binder<TagX, F> dispatch_bind(F const& f) {
    return dispatch_binder<TagX, F const>(f);
  }

#else
  /// shouldn't happen

#endif /// BOOST_PP_IS_ITERATING

