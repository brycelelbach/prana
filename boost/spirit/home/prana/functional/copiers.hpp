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
  void operator() (FromTree const&, FromKind, ToTree&, ToKind&) const; 
};

template<typename From, typename To>
inline void shallow_copier::operator() (From from, To to) const {
  to.shallow_copy(from);
}

template<typename FromTree, typename FromKind, typename ToTree, typename ToKind>
inline void shallow_copier::operator() (
  FromTree const& from_tree, FromKind from_kind,
  ToTree& to_tree, ToKind& to_kind
) const {
  to_kind = from_kind | reference_kind;

  // EXPLAIN (wash): If the source is a reference, reference the source's target
  // instead of the source itself.
  if (from_kind & reference_kind) to_tree._pointer = from_tree._pointer;

  // EXPLAIN (wash): from_tree has to be a reference to the actual tree; if it's
  // passed by value, then the address we take here would be the wrong one.
  else to_tree._pointer = &from_tree;
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
  void operator() (FromTree const&, FromKind, ToTree&, ToKind&) const; 
};

template<typename From, typename To>
inline void deep_copier::operator() (From from, To to) const {
  to.deep_copy(from);
}

template<typename FromTree, typename FromKind, typename ToTree, typename ToKind>
inline void deep_copier::operator() (
  FromTree const& from_tree, FromKind from_kind,
  ToTree& to_tree, ToKind& to_kind
) const {
  // DISCUSS (wash): If the source is a reference, copy from the referenced
  // tree. Note that a bad reference will seg fault here. By design, the user is
  // responsible for validating shallow copies. Should this behavior be changed?
  if (from_kind & reference_kind) {
    (*this)(
      *from_tree._pointer, *from_tree._pointer->_kind, to_tree, to_tree._kind
    );
    return;
  }

  to_kind = from_kind;

  switch ((kind_type) from_kind) {
    case reference_kind:
    case numeric_kind:
    case container_kind:
      // DISCUSS (wash): The above three are impossible; they fall through to
      // nil_kind for now. Perhaps they should throw an error if hit? 
    case nil_kind:
      to_tree.clear();
      break; 
    case symbol_kind:
      (*this)(from_tree._symbol, to_tree._symbol);
      break; 
    case record_kind:
      break; // TODO (wash): Requires the implementation of record. 
    case bool_kind:
      (*this)(from_tree._bool, to_tree._bool);
      break; 
    case integer_kind:
      (*this)(from_tree._integer, to_tree._integer);
      break; 
    case floating_kind:
      (*this)(from_tree._floating, to_tree._floating);
      break; 
    case sequence_kind:
      (*this)(from_tree._sequence, to_tree._sequence);
      break; 
    case array_kind:
      break; // TODO (wash): Requires the implementation of array.
    case unique_kind:
      break; // TODO (wash): Requires the implementation of unique.
  }
}

} // functional
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_FUNCTIONAL_COPIERS_HPP
