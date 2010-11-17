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

#include <boost/mpl/set.hpp>
#include <boost/mpl/order.hpp>
#include <boost/mpl/insert.hpp>

#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>

#define BOOST_SPIRIT_PRANA_INIT_TAG(_, registry, tag)                         \
  tag::size_type const tag::typeinfo =                                        \
    boost::mpl::order<registry, tag>::type::value;                            \
  /***/

#define BOOST_SPIRIT_PRANA_REGISTRY(registry, tags)                           \
  typedef boost::mpl::set<BOOST_PP_SEQ_ENUM(tags)> registry;                  \
  BOOST_PP_SEQ_FOR_EACH(BOOST_SPIRIT_PRANA_INIT_TAG, registry, tags)          \
  /***/

#define BOOST_SPIRIT_PRANA_INSERT_TAG_HEAD(old_registry, i, tag)              \
  typedef mpl::insert<old_registry, tag>::type                                \
    BOOST_PP_CAT(temp_, BOOST_PP_CAT(old_registry, 0));                       \
  /***/

#define BOOST_SPIRIT_PRANA_INSERT_TAG_TAIL(old_registry, i, tag)              \
  typedef mpl::insert<                                                        \
    BOOST_PP_CAT(temp_, BOOST_PP_CAT(old_registry, BOOST_PP_SUB(i, 1))), tag  \
  >::type BOOST_PP_CAT(temp_, BOOST_PP_CAT(old_registry, i));                 \
  /***/

#define BOOST_SPIRIT_PRANA_INSERT_TAG(_, old_registry, i, tag)                \
  BOOST_PP_IIF(BOOST_PP_BOOL(i),                                              \
    BOOST_SPIRIT_PRANA_INSERT_TAG_TAIL,                                       \
    BOOST_SPIRIT_PRANA_INSERT_TAG_HEAD)(old_registry, i, tag)                 \
  /***/

#define BOOST_SPIRIT_PRANA_EXTEND_REGISTRY(new_registry, old_registry, tags)  \
  BOOST_PP_SEQ_FOR_EACH_I(BOOST_SPIRIT_PRANA_INSERT_TAG, old_registry, tags)  \
                                                                              \
  typedef BOOST_PP_CAT(temp_,                                                 \
    BOOST_PP_CAT(old_registry, BOOST_PP_SUB(BOOST_PP_SEQ_SIZE(tags), 1)))     \
    new_registry;                                                             \
                                                                              \
  BOOST_PP_SEQ_FOR_EACH(BOOST_SPIRIT_PRANA_INIT_TAG, new_registry, tags)      \
  /***/

namespace boost {
namespace spirit {
namespace prana {

struct nil_tag {
  typedef std::size_t size_type;
  static size_type const typeinfo;
};

struct empty_list_tag {
  typedef std::size_t size_type;
  static size_type const typeinfo;
};

struct boolean_tag {
  typedef std::size_t size_type;
  static size_type const typeinfo;
};

struct integer_tag {
  typedef std::size_t size_type;
  static size_type const typeinfo;
};

struct floating_tag {
  typedef std::size_t size_type;
  static size_type const typeinfo;
};

struct cons_tag {
  typedef std::size_t size_type;
  static size_type const typeinfo;
};

struct symbol_tag {
  typedef std::size_t size_type;
  static size_type const typeinfo;
};

struct string_tag {
  typedef std::size_t size_type;
  static size_type const typeinfo;
};

BOOST_SPIRIT_PRANA_REGISTRY(
  special_objects,
  (nil_tag)
  (empty_list_tag))

BOOST_SPIRIT_PRANA_EXTEND_REGISTRY(
  numeric_tower,
  special_objects,
  (boolean_tag)
  (integer_tag)
  (floating_tag))

BOOST_SPIRIT_PRANA_EXTEND_REGISTRY(
  data_structures,
  numeric_tower,
  (cons_tag)
  (symbol_tag)
  (string_tag))

typedef data_structures core_registry;

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_REGISTRY_HPP
