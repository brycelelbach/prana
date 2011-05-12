////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_760F9FFA_B231_4C7C_9F93_401A84976732)
#define BSP_760F9FFA_B231_4C7C_9F93_401A84976732

#include <boost/spirit/home/prana/phxpr/core/unary_function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct mkdir_function: unary_function<mkdir_function> {
  mkdir_function (boost::shared_ptr<actor_list> const& elements):
    base_type(elements) { }

  typedef utree result_type;

  utree eval (utree const& subject) const;
};

struct mkdir_composite: unary_composite<mkdir_function> { };

extern mkdir_composite const mkdir_;

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_760F9FFA_B231_4C7C_9F93_401A84976732
