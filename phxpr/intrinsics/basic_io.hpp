////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_A3384D41_EC43_470E_BCE0_3D2639CEB4F3)
#define PHXPR_A3384D41_EC43_470E_BCE0_3D2639CEB4F3

#include <phxpr/config.hpp>

#include <iostream>

#include <prana/utree/predicates.hpp>
#include <prana/generate/generate_sexpr.hpp>

#include <phxpr/primitives/nullary.hpp>
#include <phxpr/primitives/unary.hpp>

namespace phxpr {

struct display: phxpr::unary<display> {
  std::ostream& out;
  const bool print_pointers;

  display (std::ostream& out_, bool print_pointers_ = true):
    out(out_), print_pointers(print_pointers_) { }

  utree eval (utree const& ut) const {
    if (prana::is_utree_string(ut)) {
      spirit::utf8_string_range_type str
        = ut.get<spirit::utf8_string_range_type>();

      BOOST_FOREACH(char c, str) { out << c; }
    }

    else 
      prana::generate_sexpr(ut, out, print_pointers);

    return utree();
  }
};

struct newline: phxpr::nullary<newline> {
  std::ostream& out;

  newline (std::ostream& out_): out(out_) { }

  utree eval (void) const {
    out << "\n";
    return utree();
  }
};

} // phxpr

#endif // PHXPR_A3384D41_EC43_470E_BCE0_3D2639CEB4F3

