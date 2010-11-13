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

  template<typename LHS, typename RHS>
  void operator() (LHS& lhs, RHS rhs) const {
    lhs.shallow_copy(rhs);
  } 

  template<class LHS, class LHS_Kind, class RHS, class RHS_Kind>
  void operator() (
    LHS& lhs, LHS_Kind& lhs_kind, RHS& rhs, RHS_Kind rhs_kind
  ) const {
    lhs.template become<nil_kind>();
    lhs_kind = rhs_kind | reference_kind;
  
    // EXPLAIN (wash): If the source is a reference, reference the source's
    // target instead of the source itself.
    // TODO (wash): This sort of logic should be moved into the du data structs
    if (rhs_kind & reference_kind)
      (*this)(lhs._du._alias, rhs._du._alias);
  
    // EXPLAIN (wash): lhs has to be a reference to the actual tree; if it's
    // passed by value, then the address we take here would be the wrong one.
    else {
      (*this)(lhs._du._alias, &rhs);
    }
  }
};

struct deep_copy {
  typedef void result_type;

  template<typename LHS, typename RHS>
  void operator() (LHS& lhs, RHS const& rhs) const {
    lhs.deep_copy(rhs);
  } 
  
  void operator() (boost::intmax_t& lhs, boost::intmax_t const& rhs) const {
    lhs = rhs;
  }  
  
  void operator() (double& lhs, double const& rhs) const {
    lhs = rhs;
  }  

  template<class LHS, class LHS_Kind, class RHS, class RHS_Kind>
  void operator() (
    LHS& lhs, LHS_Kind& lhs_kind, RHS const& rhs, RHS_Kind rhs_kind
  ) const {
    // DISCUSS (wash): If the source is a reference, copy from the referenced
    // tree. Note that a bad reference will seg fault here. By design, the user
    // is responsible for validating shallow copies. Should this behavior be
    // changed?
    if (rhs_kind & reference_kind) {
      // DISCUSS (wash): This is pretty horrible to my eyes, copiers ought to be
      // rewritten to not dispatch usng kind references.
      (*this)(lhs, lhs._du._alias._data._control[0],
              *rhs._du._alias,
              rhs._du._alias->_du._alias._data._control[0]);
      return;
    }
  
    lhs_kind = rhs_kind;
  
    switch ((kind_type) rhs_kind) {
      case nil_kind:
        lhs.template become<nil_kind>();
        return;
      case symbol_kind:
        lhs.template become<symbol_kind>();
        (*this)(lhs._du._symbol, rhs._du._symbol);
        return; 
      case integer_kind:
        lhs.template become<integer_kind>();
        (*this)(lhs._du._numeric._data._integer,
                rhs._du._numeric._data._integer);
        return; 
      case floating_kind:
        lhs.template become<floating_kind>();
        (*this)(lhs._du._numeric._data._floating,
                rhs._du._numeric._data._floating);
        return; 
      case sequence_kind:
        lhs.template become<sequence_kind>();
        (*this)(lhs._du._sequence, rhs._du._sequence);
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