////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_2FDDB2FF_5730_4AD1_9330_F2A4A379B394)
#define BSP_2FDDB2FF_5730_4AD1_9330_F2A4A379B394

#include <boost/spirit/home/prana/phxpr/core/binary_function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct symlink_function: binary_function<symlink_function> {
  symlink_function (boost::shared_ptr<actor_list> const& elements):
    base_type(elements) { }

  typedef utree result_type;

  utree eval (utree const& source, utree const& dest) const;
};

struct symlink_composite: binary_composite<symlink_function> { };

extern symlink_composite const symlink_;

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_2FDDB2FF_5730_4AD1_9330_F2A4A379B394

