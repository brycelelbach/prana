////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_E5EBFDC5_F832_49E1_95CE_80F0D5C72FC7)
#define PHXPR_E5EBFDC5_F832_49E1_95CE_80F0D5C72FC7

#include <phxpr/config.hpp>

#include <ostream>

#include <boost/cstdint.hpp>

#include <boost/fusion/include/vector.hpp>

namespace phxpr {

typedef std::size_t displacement;

struct arity_type {
  enum info {
    fixed    = 0,
    variable = 1
  };

  typedef boost::uint8_t integral;
};

struct evaluation_strategy {
  enum info {
    call_by_value = 0
  };

  typedef boost::uint8_t integral;
};

struct function_type {
  enum info {
    derived     = 0,
    lambda      = 1,
    placeholder = 2,
    procedure   = 3,
    thunk       = 4,
    module      = 5,
    conditional = 6
  };

  typedef boost::uint8_t integral;
};

typedef boost::fusion::vector4<
  displacement, arity_type::info, evaluation_strategy::info, function_type::info
> signature;

// {{{ streaming operators
std::ostream&
operator<< (std::ostream& out, function_type::info const& info) PHXPR_EXPORT;

std::ostream&
operator<< (std::ostream& out, arity_type::info const& info) PHXPR_EXPORT;

std::ostream& 
operator<< (std::ostream& out, evaluation_strategy::info const& info)
PHXPR_EXPORT;
// }}}

} // phxpr

#endif // PHXPR_E5EBFDC5_F832_49E1_95CE_80F0D5C72FC7

