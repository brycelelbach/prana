/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_19F8ECFF_8EBE_40A3_AA16_BD351541E363)
#define PRANA_19F8ECFF_8EBE_40A3_AA16_BD351541E363

#include <prana/include/utree.hpp>

namespace prana {

template <typename T>
struct utree_function_caster {
  typedef T const* result_type;

  // forwarder
  result_type operator() (utree const& ut) const {
    return utree::visit(ut, *this);
  }

  template <typename Other>
  result_type operator() (Other const&) const {
    return 0;
  }

  result_type operator() (function_base const& pf) const {
    return dynamic_cast<result_type>(&pf);
  }
};

template <typename T>
T const* utree_function_cast (utree const& ut) {
  return utree::visit(ut, utree_function_caster<T>());
}

} // prana

#endif // PRANA_19F8ECFF_8EBE_40A3_AA16_BD351541E363

