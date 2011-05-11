/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_E35A5F52_FC64_4523_930E_825FE612E465)
#define BSP_E35A5F52_FC64_4523_930E_825FE612E465

#include <boost/spirit/home/prana/phxpr/core/binary_function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct push_front_function: binary_function<push_front_function> {
  push_front_function (boost::shared_ptr<actor_list> const& elements):
    base_type(elements) { }

  typedef utree result_type;

  result_type eval (utree const& value, utree const& list) const {
    utree copy(list);
    copy.push_front(value); 
    return copy;
  }
};

struct push_front_composite: binary_composite<push_front_function> { };

push_front_composite const push_front = push_front_composite();

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_E35A5F52_FC64_4523_930E_825FE612E465

