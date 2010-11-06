/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_FN_COPY_HPP)
#define BOOST_SPIRIT_PRANA_FN_COPY_HPP

#include <boost/spirit/home/prana/kind.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace fn {

struct shallow_copy {
  typedef void result_type;

  template<typename LHS, typename RHS>
  void operator() (LHS lhs, RHS rhs) const {
    lhs.shallow_copy(rhs);
  } 

  template<class LHS, class LHS_Kind, class RHS, class RHS_Kind>
  void operator() (
    LHS const& lhs, LHS_Kind lhs_kind, RHS& rhs, RHS_Kind& rhs_kind
  ) const { 
    rhs_kind = lhs_kind | reference_kind;
  
    // EXPLAIN (wash): If the source is a reference, reference the source's
    // target instead of the source itself.
    if (lhs_kind & reference_kind)
      rhs.raw()._pointer = lhs.raw()._pointer;
  
    // EXPLAIN (wash): lhs has to be a reference to the actual tree; if it's
    // passed by value, then the address we take here would be the wrong one.
    else rhs.raw()._pointer = &lhs;
  }
};

struct deep_copy {
  typedef void result_type;

  template<typename LHS, typename RHS>
  void operator() (LHS lhs, RHS rhs) const {
    lhs.deep_copy(rhs);
  } 
  
  template<class LHS, class LHS_Kind, class RHS, class RHS_Kind>
  void operator() (
    LHS const& lhs, LHS_Kind lhs_kind, RHS& rhs, RHS_Kind& rhs_kind
  ) const {
    // DISCUSS (wash): If the source is a reference, copy from the referenced
    // tree. Note that a bad reference will seg fault here. By design, the user
    // is responsible for validating shallow copies. Should this behavior be
    // changed?
    if (lhs_kind & reference_kind) {
      (*this)(*lhs.raw()._pointer, *lhs.raw()._pointer->kind(), rhs, rhs.kind());
      return;
    }
  
    rhs_kind = lhs_kind;
  
    switch ((kind_type) lhs_kind) {
      case reference_kind:
      case numeric_kind:
      case container_kind:
        // DISCUSS (wash): The above three are impossible; they fall through to
        // nil_kind for now. Perhaps they should throw an error if hit? 
      case nil_kind:
        rhs.clear();
        break; 
      case symbol_kind:
        (*this)(lhs.raw()._symbol, rhs.raw()._symbol);
        break; 
      case bool_kind:
        (*this)(lhs.raw()._bool, rhs.raw()._bool);
        break; 
      case integer_kind:
        (*this)(lhs.raw()._integer, rhs.raw()._integer);
        break; 
      case floating_kind:
        (*this)(lhs.raw()._floating, rhs.raw()._floating);
        break; 
      case sequence_kind:
        (*this)(lhs.raw()._sequence, rhs.raw()._sequence);
        break; 
      case record_kind:
        break; // TODO (wash): Requires the implementation of record. 
      case array_kind:
        break; // TODO (wash): Requires the implementation of array.
      case unique_kind:
        break; // TODO (wash): Requires the implementation of unique.
    }
  }
};

} // fn
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_FN_COPY_HPP
