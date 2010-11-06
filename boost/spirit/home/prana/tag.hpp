/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TAG_HPP)
#define BOOST_SPIRIT_PRANA_TAG_HPP

#include <boost/spirit/home/support/terminal.hpp>

#include <boost/spirit/home/prana/fn/copy.hpp>
#include <boost/spirit/home/prana/fn/front.hpp>
#include <boost/spirit/home/prana/fn/back.hpp>

#define BOOST_SPIRIT_PRANA_TAG(type, name)                            \
  type const name = {};                                               \
  inline void silence_unused_warnings_##name (void) { (void) name; }  \
  /***/

#define BOOST_SPIRIT_PRANA_DEFINE_TAGS_A(r, _, name)                  \
  BOOST_SPIRIT_PRANA_TAG name                                         \
  /***/

#define BOOST_SPIRIT_PRANA_DEFINE_TAGS(seq)                           \
  BOOST_PP_SEQ_FOR_EACH(BOOST_SPIRIT_PRANA_DEFINE_TAGS_A, _, seq)     \
  /***/

namespace boost {
namespace spirit {
namespace prana {

BOOST_SPIRIT_PRANA_DEFINE_TAGS(
  ((fn::shallow_copy, shallow))
  ((fn::deep_copy,    deep))
  ((fn::front,        front))
  ((fn::back,         back))
)

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TAG_HPP
