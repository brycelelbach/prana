////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_04896866_F86E_4968_96AE_2DC05441BCFA)
#define PHXPR_04896866_F86E_4968_96AE_2DC05441BCFA

#include <phxpr/primitives/binary.hpp>

namespace phxpr {

struct less_predicate: phxpr::binary<less_predicate> {
  utree eval (utree const& lhs, utree const& rhs) const
  { return lhs < rhs; }
  
  function_base* duplicate (void) const
  { return new less_predicate; } 
};

struct less_equal_predicate: phxpr::binary<less_equal_predicate> {
  utree eval (utree const& lhs, utree const& rhs) const
  { return lhs <= rhs; }
  
  function_base* duplicate (void) const
  { return new less_equal_predicate; } 
};

struct greater_predicate: phxpr::binary<greater_predicate> {
  utree eval (utree const& lhs, utree const& rhs) const
  { return lhs > rhs; }
  
  function_base* duplicate (void) const
  { return new greater_predicate; } 
};

struct greater_equal_predicate: phxpr::binary<greater_equal_predicate> {
  utree eval (utree const& lhs, utree const& rhs) const
  { return lhs >= rhs; }
  
  function_base* duplicate (void) const
  { return new greater_equal_predicate; } 
};

} // phxpr

#endif // PHXPR_04896866_F86E_4968_96AE_2DC05441BCFA

