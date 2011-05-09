////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_A7136DD6_5C8E_4A79_BD18_85EBE176C56C)
#define BSP_A7136DD6_5C8E_4A79_BD18_85EBE176C56C

#include <boost/spirit/home/prana/phxpr/core/unary_function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct remove_function: unary_function<remove_function> {
  remove_function (boost::shared_ptr<actor_list> const& elements):
    base_type(elements) { }

  typedef utree result_type;

  utree eval (utree const& subject) const;
};

struct remove_composite: unary_composite<remove_function> { };

extern remove_composite const remove_;

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_A7136DD6_5C8E_4A79_BD18_85EBE176C56C

