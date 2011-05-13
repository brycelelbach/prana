/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_19F8ECFF_8EBE_40A3_AA16_BD351541E363)
#define BSP_19F8ECFF_8EBE_40A3_AA16_BD351541E363

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

// TODO: :( dynamic_cast
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
} // spirit
} // boost

#endif // BSP_19F8ECFF_8EBE_40A3_AA16_BD351541E363

