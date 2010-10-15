/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_REST_HPP)
#define BOOST_SPIRIT_PRANA_REST_HPP

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/spirit/home/prana/shallow.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct rester {
  template<typename Tree> struct result;

  template<typename Tree>
  struct result<Tree&> { typedef Tree type; };

  template<typename Tree>
  struct result<Tree const&> { typedef Tree type; };

  template<typename Tree>
  Tree operator() (Tree& x) const {
    typename Tree::iterator i = x.begin(); ++i;
    return Tree(typename Tree::range(i, x.end()), shallow);
  }

  template<typename Tree>
  Tree operator() (Tree const& x) const {
    typename Tree::const_iterator i = x.begin(); ++i;
    return Tree(typename Tree::const_range(i, x.end()), shallow);
  }
};

template<typename Tree>
typename rester::result<Tree&>::type rest (Tree& x) {
  rester r;
  return r(x);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_REST_HPP

