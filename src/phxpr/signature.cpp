////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <phxpr/signature.hpp>

namespace phxpr {

std::ostream&
operator<< (std::ostream& out, arity_type::info const& info) {
  switch (info) {
    case arity_type::fixed:
      { out << "fixed"; return out; }
    case arity_type::variable:
      { out << "variable"; return out; }
    default:
      { out << "unknown"; return out; }
  };
}

std::ostream&
operator<< (std::ostream& out, evaluation_strategy::info const& info) {
  switch (info) {
    case evaluation_strategy::call_by_value:
      { out << "call_by_value"; return out; }
    default:
      { out << "unknown"; return out; }
  };
}

std::ostream&
operator<< (std::ostream& out, function_type::info const& info) {
  switch (info) {
    case function_type::derived:
      { out << "derived"; return out; }
    case function_type::lambda:
      { out << "lambda"; return out; }
    case function_type::placeholder:
      { out << "placeholder"; return out; }
    case function_type::procedure:
      { out << "procedure"; return out; }
    default:
      { out << "unknown"; return out; }
  };
}

} // phxpr

