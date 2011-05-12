/*==============================================================================
    Copyright (c) 2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_45BF9052_FF1F_4F22_BB94_AF20DF631795)
#define BSP_45BF9052_FF1F_4F22_BB94_AF20DF631795

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

utree_type::info recursive_which (utree const& ut) {
  if (ut.which() == utree_type::reference_type)
    return recursive_which(ut.deref());

  return ut.which();
}

bool is_utree_string (utree const& ut) {
  utree_type::info t = recursive_which(ut);
  return t == utree_type::string_type || t == utree_type::string_range_type;
}

bool is_utree_container (utree const& ut) {
  utree_type::info t = recursive_which(ut);
  return t == utree_type::list_type || t == utree_type::range_type;
}

} // prana
} // spirit
} // boost

#endif // BSP_45BF9052_FF1F_4F22_BB94_AF20DF631795

