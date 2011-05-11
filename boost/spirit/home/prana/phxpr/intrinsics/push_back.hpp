/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_4447D563_8260_49BA_BD18_C59325011206)
#define BSP_4447D563_8260_49BA_BD18_C59325011206

#include <boost/spirit/home/prana/phxpr/core/binary_function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct push_back_function: binary_function<push_back_function> {
  push_back_function (boost::shared_ptr<actor_list> const& elements):
    base_type(elements) { }

  typedef utree result_type;

  result_type eval (utree const& value, utree const& list) const {
    utree copy(list);
    copy.push_back(value); 
    return copy;
  }
};

struct push_back_composite: binary_composite<push_back_function> { };

push_back_composite const push_back = push_back_composite();

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_4447D563_8260_49BA_BD18_C59325011206

