////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_CB6EF60D_F767_4679_A03C_279852709DB3)
#define PHXPR_CB6EF60D_F767_4679_A03C_279852709DB3

#include <phxpr/primitives/binary.hpp>

namespace phxpr {

struct addition: phxpr::binary<addition> {
  utree eval (utree const& lhs, utree const& rhs) const
  { return lhs + rhs; }
};

struct subtraction: phxpr::binary<subtraction> {
  utree eval (utree const& lhs, utree const& rhs) const
  { return lhs - rhs; }
};

struct multiplication: phxpr::binary<multiplication> {
  utree eval (utree const& lhs, utree const& rhs) const
  { return lhs * rhs; }
};

struct division: phxpr::binary<division> {
  utree eval (utree const& lhs, utree const& rhs) const
  { return lhs / rhs; }
};

} // phxpr

#endif // PHXPR_CB6EF60D_F767_4679_A03C_279852709DB3

