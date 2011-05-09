////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_FDAE0AD2_D72E_40C0_BC4C_F92F16E3A9D9)
#define BSP_FDAE0AD2_D72E_40C0_BC4C_F92F16E3A9D9

#include <boost/spirit/home/prana/phxpr/core/unary_function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct symlink_predicate_function: unary_function<symlink_predicate_function> {
  symlink_predicate_function (boost::shared_ptr<actor_list> const& elements):
    base_type(elements) { }

  typedef utree result_type;

  utree eval (utree const& subject) const;
};

struct symlink_predicate_composite:
  unary_composite<symlink_predicate_function> { };

extern symlink_predicate_composite const is_link;

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_FDAE0AD2_D72E_40C0_BC4C_F92F16E3A9D9

