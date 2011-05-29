////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_02C00BFB_2E61_4213_A887_4F12570B22D3)
#define PHXPR_02C00BFB_2E61_4213_A887_4F12570B22D3

#include <phxpr/primitives/unary.hpp>
#include <phxpr/predicate.hpp>
#include <phxpr/exception.hpp>

namespace phxpr {

struct assertion: phxpr::unary<assertion> {
  utree eval (utree const& subject) const { 
    if (!predicate(subject))
      BOOST_THROW_EXCEPTION(assertion_failed());
    return utree();
  } 
};

} // phxpr

#endif // PHXPR_02C00BFB_2E61_4213_A887_4F12570B22D3

