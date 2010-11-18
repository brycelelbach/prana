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

#define BOOST_SPIRIT_PRANA_INIT_TAG(_, registry, tag)                         \
  tag::value_type const tag::value =                                          \
    boost::mpl::order<registry, tag>::type::value;                            \
  /***/

#define BOOST_SPIRIT_PRANA_REGISTRY_SET(registry, tags)                       \
  typedef boost::mpl::set<BOOST_PP_SEQ_ENUM(tags)> registry;                  \
  /***/

#define BOOST_SPIRIT_PRANA_INIT_TAGS(registry, tags)                          \
  BOOST_PP_SEQ_FOR_EACH(BOOST_SPIRIT_PRANA_INIT_TAG, registry, tags)          \
  /***/

#define BOOST_SPIRIT_PRANA_INSERT_TAG_HEAD(old_reg, i, tag)                   \
  typedef mpl::insert<old_reg, tag>::type                                     \
    BOOST_PP_CAT(temp_, BOOST_PP_CAT(old_reg, 0));                            \
  /***/

#define BOOST_SPIRIT_PRANA_INSERT_TAG_TAIL(old_reg, i, tag)                   \
  typedef mpl::insert<                                                        \
    BOOST_PP_CAT(temp_, BOOST_PP_CAT(old_reg, BOOST_PP_SUB(i, 1))), tag       \
  >::type BOOST_PP_CAT(temp_, BOOST_PP_CAT(old_reg, i));                      \
  /***/

#define BOOST_SPIRIT_PRANA_INSERT_TAG(_, old_reg, i, tag)                     \
  BOOST_PP_IIF(BOOST_PP_BOOL(i),                                              \
    BOOST_SPIRIT_PRANA_INSERT_TAG_TAIL,                                       \
    BOOST_SPIRIT_PRANA_INSERT_TAG_HEAD)(old_reg, i, tag)                      \
  /***/

#define BOOST_SPIRIT_PRANA_EXTEND_REGISTRY_SET(new_reg, old_reg, tags)        \
  BOOST_PP_SEQ_FOR_EACH_I(BOOST_SPIRIT_PRANA_INSERT_TAG, old_reg, tags)       \
                                                                              \
  typedef BOOST_PP_CAT(temp_,                                                 \
    BOOST_PP_CAT(old_reg, BOOST_PP_SUB(BOOST_PP_SEQ_SIZE(tags), 1)))          \
    new_reg;                                                                  \
  /***/

#define BOOST_SPIRIT_PRANA_TAG(r, v, elem)                                    \
  struct BOOST_PP_SEQ_ELEM(0, elem) {                                         \
    typedef v value_type;                                                     \
    static value_type const value;                                            \
    typedef BOOST_PP_SEQ_ELEM(1, elem) type;                                  \
  };                                                                          \
  /***/

#define BOOST_SPIRIT_PRANA_TAGS(v, tags)                                      \
  BOOST_PP_SEQ_FOR_EACH(BOOST_SPIRIT_PRANA_TAG, v, tags)                      \
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
