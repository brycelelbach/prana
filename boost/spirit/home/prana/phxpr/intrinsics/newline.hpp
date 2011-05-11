/*==============================================================================
    Copyright (c) 2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_860EC52A_BE13_40B1_B45A_BB34B41B44D6)
#define BSP_860EC52A_BE13_40B1_B45A_BB34B41B44D6

#include <boost/spirit/home/prana/phxpr/core/composite.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

// temporary intrinsic, superseded by ports
struct newline_function: actor<newline_function> {
  newline_function (void) { }

  newline_function (boost::shared_ptr<actor_list> const&) { }

  utree eval (scope const&) const {
    std::cout << std::endl;
    return utree(spirit::nil);
  }
};

struct newline_composite: composite<newline_composite> {
  function compose (boost::shared_ptr<actor_list> const&) const {
    return function(newline_function());
  }
};

newline_composite const newline = newline_composite();

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_860EC52A_BE13_40B1_B45A_BB34B41B44D6

