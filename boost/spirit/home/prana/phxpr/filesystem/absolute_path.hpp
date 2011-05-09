////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_4524ED1A_BDF3_435A_8B7E_03F6A82B19AD)
#define BSP_4524ED1A_BDF3_435A_8B7E_03F6A82B19AD

#include <boost/spirit/home/prana/phxpr/core/unary_function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct absolute_path_function: unary_function<absolute_path_function> {
  absolute_path_function (boost::shared_ptr<actor_list> const& elements):
    base_type(elements) { }

  typedef utree result_type;

  utree eval (utree const& subject) const;
};

struct absolute_path_composite: unary_composite<absolute_path_function> { };

extern absolute_path_composite const abspath;

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_4524ED1A_BDF3_435A_8B7E_03F6A82B19AD

