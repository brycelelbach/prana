////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_2F9A7918_D1BE_49F1_BA9C_47C2CB1F3BD1)
#define PHXPR_2F9A7918_D1BE_49F1_BA9C_47C2CB1F3BD1

#include <phxpr/config.hpp>

#include <boost/throw_exception.hpp>

#include <phxpr/signature.hpp>

namespace phxpr {

struct exception: virtual std::exception { };

struct invalid_arity: virtual exception {
  displacement expected;
  displacement got;
  arity_type::info type;

  invalid_arity (displacement expected_, displacement got_,
                 arity_type::info type_):
    expected(expected_), got(got_), type(type_) { }
};

struct invalid_local_variable: virtual exception {
  displacement expected;
  displacement got;

  invalid_local_variable (displacement expected_, displacement got_):
    expected(expected_), got(got_) { }
};

struct unsupported_arity_type: virtual exception {
  arity_type::info type;

  unsupported_arity_type (arity_type::info type_): type(type_) { }
};

} // phxpr

#endif // PHXPR_2F9A7918_D1BE_49F1_BA9C_47C2CB1F3BD1

