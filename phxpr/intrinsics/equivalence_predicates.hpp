////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_BBD50794_5DBA_4E65_823C_8CD9EA172073)
#define PHXPR_BBD50794_5DBA_4E65_823C_8CD9EA172073

#include <phxpr/primitives/binary.hpp>

namespace phxpr {

struct equal_predicate: phxpr::binary<equal_predicate> {
  utree eval (utree const& lhs, utree const& rhs) const
  { return lhs == rhs; }
};

} // phxpr

#endif // PHXPR_BBD50794_5DBA_4E65_823C_8CD9EA172073

