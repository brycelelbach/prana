/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_REGISTRY_HPP)
#define BOOST_SPIRIT_PRANA_REGISTRY_HPP

#include <boost/config.hpp>
#include <boost/assert.hpp>

#include <boost/mpl/set.hpp>
#include <boost/mpl/order.hpp>
#include <boost/mpl/insert.hpp>

#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>

/* INIT_TYPES and TEMP_INIT_TYPES implementation */

#define BOOST_SPIRIT_PRANA_SEQ_ADD(_, data, elem)                             \
  data elem                                                                   \
  /***/

#define BOOST_SPIRIT_PRANA_INIT_TYPE(_, registry, tag)                        \
  tag::value_type const tag::value =                                          \
    boost::mpl::order<registry, tag>::type::value;                            \
  /***/

#define BOOST_SPIRIT_PRANA_NNS_INIT_TYPE(_, data, tag)                        \
  BOOST_PP_SEQ_ELEM(0, data)::tag::value_type const                           \
    BOOST_PP_SEQ_ELEM(0, data)::tag::value =                                  \
      boost::mpl::order<                                                      \
        BOOST_PP_SEQ_ELEM(0, data)::BOOST_PP_SEQ_ELEM(1, data),               \
        BOOST_PP_SEQ_ELEM(0, data)::tag                                       \
      >::type::value;                                                         \
  /***/

#define BOOST_SPIRIT_PRANA_TEMP_INIT_TYPE(s, data, tag)                       \
  template<BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM_S(s,                      \
    BOOST_SPIRIT_PRANA_SEQ_ADD, class, BOOST_PP_SEQ_ELEM(1, data)))>          \
  typename                                                                    \
    BOOST_PP_SEQ_ELEM(0, data)<BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_ELEM(1, data))> \
  ::tag::value_type const                                                     \
    BOOST_PP_SEQ_ELEM(0, data)<BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_ELEM(1, data))> \
  ::tag::value =                                                              \
    boost::mpl::order<                                                        \
      typename BOOST_PP_SEQ_ELEM(0, data)                                     \
        <BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_ELEM(1, data))>                       \
        ::BOOST_PP_SEQ_ELEM(2, data),                                         \
      typename BOOST_PP_SEQ_ELEM(0, data)                                     \
        <BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_ELEM(1, data))>::tag                  \
    >::type::value;                                                           \
  /***/

/* INIT_TYPES and TEMP_INIT_TYPES */

#define BOOST_SPIRIT_PRANA_INIT_TYPES(registry, tags)                         \
  BOOST_PP_SEQ_FOR_EACH(BOOST_SPIRIT_PRANA_INIT_TYPE, registry, tags)         \
  /***/

#define BOOST_SPIRIT_PRANA_NNS_INIT_TYPES(nns, registry, tags)                \
  BOOST_PP_SEQ_FOR_EACH(                                                      \
    BOOST_SPIRIT_PRANA_NNS_INIT_TYPE, (nns) (registry), tags)                 \
  /***/

#define BOOST_SPIRIT_PRANA_TEMP_INIT_TYPES(class_, params, registry, tags)    \
  BOOST_PP_SEQ_FOR_EACH(                                                      \
    BOOST_SPIRIT_PRANA_TEMP_INIT_TYPE, (class_) (params) (registry), tags)    \
  /***/

/* REGISTRY_SET and TEMP_REGISTRY_SET */

#define BOOST_SPIRIT_PRANA_REGISTRY_SET(registry, tags)                       \
  typedef boost::mpl::set<BOOST_PP_SEQ_ENUM(tags)> registry;                  \
  /***/

#define BOOST_SPIRIT_PRANA_TEMP_REGISTRY_SET(registry, tags)                  \
  typedef typename boost::mpl::set<BOOST_PP_SEQ_ENUM(tags)> registry;         \
  /***/

/* EXTEND_REGISTRY_SET and TEMP_EXTEND_REGISTRY_SET implementation */

#define BOOST_SPIRIT_PRANA_INSERT_TAG_HEAD(old, i, tag)                       \
  typedef mpl::insert<old, tag>::type                                         \
    BOOST_PP_CAT(temp_, BOOST_PP_CAT(old, 0));                                \
  /***/

#define BOOST_SPIRIT_PRANA_INSERT_TAG_TAIL(old, i, tag)                       \
  typedef mpl::insert<                                                        \
    BOOST_PP_CAT(temp_, BOOST_PP_CAT(old, BOOST_PP_SUB(i, 1))), tag           \
  >::type BOOST_PP_CAT(temp_, BOOST_PP_CAT(old, i));                          \
  /***/

#define BOOST_SPIRIT_PRANA_INSERT_TAG(_, old, i, tag)                         \
  BOOST_PP_IIF(BOOST_PP_BOOL(i),                                              \
    BOOST_SPIRIT_PRANA_INSERT_TAG_TAIL,                                       \
    BOOST_SPIRIT_PRANA_INSERT_TAG_HEAD)(old, i, tag)                          \
  /***/

#define BOOST_SPIRIT_PRANA_TEMP_INSERT_TAG_HEAD(old, i, tag)                  \
  typedef typename mpl::insert<old, tag>::type                                \
    BOOST_PP_CAT(temp_, BOOST_PP_CAT(old, 0));                                \
  /***/

#define BOOST_SPIRIT_PRANA_TEMP_INSERT_TAG_TAIL(old, i, tag)                  \
  typedef typename mpl::insert<                                               \
    BOOST_PP_CAT(temp_, BOOST_PP_CAT(old, BOOST_PP_SUB(i, 1))), tag           \
  >::type BOOST_PP_CAT(temp_, BOOST_PP_CAT(old, i));                          \
  /***/

#define BOOST_SPIRIT_PRANA_TEMP_INSERT_TAG(_, old, i, tag)                    \
  BOOST_PP_IIF(BOOST_PP_BOOL(i),                                              \
    BOOST_SPIRIT_PRANA_TEMP_INSERT_TAG_TAIL,                                  \
    BOOST_SPIRIT_PRANA_TEMP_INSERT_TAG_HEAD)(old, i, tag)                     \
  /***/

/* EXTEND_REGISTRY_SET and TEMP_EXTEND_REGISTRY_SET */

#define BOOST_SPIRIT_PRANA_EXTEND_REGISTRY_SET(new_, this_, old, tags)        \
  BOOST_PP_SEQ_FOR_EACH_I(BOOST_SPIRIT_PRANA_INSERT_TAG, old, tags)           \
                                                                              \
  BOOST_SPIRIT_PRANA_REGISTRY_SET(this_, tags)                                \
                                                                              \
  typedef BOOST_PP_CAT(temp_,                                                 \
    BOOST_PP_CAT(old, BOOST_PP_SUB(BOOST_PP_SEQ_SIZE(tags), 1)))              \
    new_;                                                                     \
  /***/

#define BOOST_SPIRIT_PRANA_TEMP_EXTEND_REGISTRY_SET(new_, this_, old, tags)   \
  BOOST_PP_SEQ_FOR_EACH_I(BOOST_SPIRIT_PRANA_TEMP_INSERT_TAG, old, tags)      \
                                                                              \
  BOOST_SPIRIT_PRANA_TEMP_REGISTRY_SET(this_, tags)                           \
                                                                              \
  typedef BOOST_PP_CAT(temp_,                                                 \
    BOOST_PP_CAT(old, BOOST_PP_SUB(BOOST_PP_SEQ_SIZE(tags), 1)))              \
    new_;                                                                     \
  /***/

/* TYPES implementation */

#define BOOST_SPIRIT_PRANA_TYPE(r, v, elem)                                   \
  struct BOOST_PP_SEQ_ELEM(0, elem) {                                         \
    typedef v value_type;                                                     \
    static value_type const value;                                            \
    typedef BOOST_PP_SEQ_ELEM(1, elem) type;                                  \
  };                                                                          \
  /***/

/* TYPES */

#define BOOST_SPIRIT_PRANA_TYPES(v, tags)                                     \
  BOOST_PP_SEQ_FOR_EACH(BOOST_SPIRIT_PRANA_TYPE, v, tags)                     \
  /***/

namespace boost {
namespace spirit {
namespace prana {

template<typename RegistrySet>
struct basic_registry: RegistrySet {
  template<class X>
  struct which {
    typedef typename X::typeinfo result_type;

    result_type operator() (X& x) const {
      return x.type;
    }
  };

  template<class X, class Y = X>
  struct default_ {
    typedef void result_type;

    template<class F>
    void operator() (F, X&) const {
      BOOST_ASSERT(!"(error (\"invalid type information\"))");
    };
    
    template<class F>
    void operator() (F, X&, Y&) const {
      BOOST_ASSERT(!"(error (\"invalid type information\"))");
    };
  };

  template<class TagX, class TagY = TagX>
  struct fallthrough: mpl::bool_<false> { }; 
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_REGISTRY_HPP
