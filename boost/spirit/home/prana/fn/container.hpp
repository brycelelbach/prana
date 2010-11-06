/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_FN_CONTAINER_HPP)
#define BOOST_SPIRIT_PRANA_FN_CONTAINER_HPP

#include <boost/spirit/home/prana/kind.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace fn {

struct is_default_container {
  typedef bool result_type;

  template<class Tree>
  bool operator() (Tree const& tree) const {
    return tree.kind == Tree::policy::container::kind::value;
  }
};

struct is_container {
  typedef bool result_type;

  template<class Tree>
  bool operator() (Tree const& tree) const {
    return tree.kind & container_kind;
  }
  
  bool operator() (kind_type kind) const {
    return kind & container_kind;
  }
};

struct to_nil {
  typedef void result_type;

  template<class Tree, class F>
  void operator() (Tree& tree, F f) const {
    typedef typename Tree::policy::container container;

    Tree old_(tree, deep);
    tree.clear();
    tree.become(container::kind); 
    f(tree, old_);
  } 
};

struct to_container {
  typedef void result_type;

  template<class Tree, class F>
  void operator() (Tree& tree, F f) const {
    typedef typename Tree::policy::container container;

    Tree old_(tree, deep);
    tree.clear();
    tree.become(container::kind); 
    f(tree, old_);
  } 
};

template<class Transform, class Predicate = is_default_container>
struct with_transform {
  // TODO (wash): Return should be F's return type for maximum extensibility
  typedef void result_type;

  template<class Tree, class F>
  void operator() (Tree& lhs, Tree const& rhs, F f) const {
    if (lhs.kind() & reference_kind) {
      (*this)(*lhs.raw()._pointer, rhs, f);
      return;
    }

    // TODO (wash): remove !
    if (!Predicate()(lhs)) Transform()(lhs, f); 
    f(lhs, rhs);
  }
};

template<class Transform, class Predicate = is_container>
struct or_transform {
  // TODO (wash): Return should be F's return type for maximum extensibility
  typedef void result_type;

  template<class Tree, class F>
  void operator() (Tree& lhs, Tree const& rhs, F f) const {
    if (lhs.kind() & reference_kind) {
      (*this)(*lhs.raw()._pointer, rhs, f);
      return;
    }

    // TODO (wash): remove !
    if (!Predicate()(lhs)) Transform()(lhs, f);
    else f(lhs, rhs);
  }
};

} // fn
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_FN_CONTAINER_HPP
