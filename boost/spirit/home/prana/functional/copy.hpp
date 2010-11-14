/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_FN_COPY_HPP)
#define BOOST_SPIRIT_PRANA_FN_COPY_HPP

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>

#include <boost/spirit/home/prana/kind.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct shallow_copy {
  typedef void result_type;

  template<class LHS, class RHS>
  void operator() (LHS& lhs, RHS& rhs) const {
    // EXPLAIN (wash): If the source is a reference, reference the source's
    // target instead of the source itself.
    if (rhs.kind() & reference_kind)
      return lhs._du._alias.copy(rhs._du._alias);
  
    // EXPLAIN (wash): lhs has to be a reference to the actual tree; if it's
    // passed by value, then the address we take here would be the wrong one.
    lhs._du._alias.copy(&rhs);
  }
};

struct deep_copy {
  typedef void result_type;

  template<class LHS, class RHS>
  void operator() (LHS& lhs, RHS const& rhs) const {
    // DISCUSS (wash): If the source is a reference, copy from the referenced
    // tree. Note that a bad reference will seg fault here. By design, the user
    // is responsible for validating shallow copies. Should this behavior be
    // changed?
    if (lhs.kind() & reference_kind) 
      return (*this)(*lhs._du._alias, rhs);

    if (rhs.kind() & reference_kind) 
      return (*this)(lhs, *rhs._du._alias);
  
    switch ((kind_type) rhs.kind()) {
      case nil_kind:
        lhs = 0;
        return;
      case symbol_kind:
        lhs._du._symbol.deep_copy(rhs._du._symbol);
        return; 
      case integer_kind:
        lhs._du._numeric.copy(rhs._du._numeric);
        return; 
      case floating_kind:
        lhs._du._numeric.copy(rhs._du._numeric);
        return; 
      case sequence_kind:
        lhs._du._sequence.deep_copy(rhs._du._sequence);
        return; 
      case record_kind:
        return; // TODO (wash): Requires the implementation of record. 
      default: BOOST_ASSERT("kind is only valid when masking another kind");
    }
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_FN_COPY_HPP
