/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_FUNCTIONAL_COPIERS_HPP)
#define BOOST_SPIRIT_PRANA_FUNCTIONAL_COPIERS_HPP

#include <boost/cstdint.hpp>

#include <boost/spirit/home/prana/kinds.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace functional {

struct shallow_copier {
  template<
    typename A, typename B, typename C = unused_type, typename D = unused_type
  >
  struct result { typedef void type; };

  template<typename From, typename To>
  void operator() (From, To) const;

  template<
    typename FromTree, typename FromKind, typename ToTree, typename ToKind
  >
  void operator() (FromTree, FromKind, ToTree&, ToKind&) const; 
};

template<typename From, typename To>
inline void shallow_copier::operator() (From from, To to) const {
  to.shallow_copy(from);
}

template<typename FromTree, typename FromKind, typename ToTree, typename ToKind>
inline void shallow_copier::operator() (
  FromTree from_tree, FromKind from_kind, ToTree& to_tree, ToKind& to_kind
) const {
  to_kind = from_kind | reference_kind;
}

struct deep_copier {
  template<
    typename A, typename B, typename C = unused_type, typename D = unused_type
  >
  struct result { typedef void type; };

  template<typename From, typename To>
  void operator() (From, To) const;
  
  template<
    typename FromTree, typename FromKind, typename ToTree, typename ToKind
  >
  void operator() (FromTree, FromKind, ToTree&, ToKind&) const; 
};

template<typename From, typename To>
inline void deep_copier::operator() (From from, To to) const {
  to.deep_copy(from);
}

template<typename FromTree, typename FromKind, typename ToTree, typename ToKind>
inline void deep_copier::operator() (
  FromTree from_tree, FromKind from_kind, ToTree& to_tree, ToKind& to_kind
) const {
  to_kind = from_kind | ~reference_kind;
}

} // functional
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_FUNCTIONAL_COPIERS_HPP
