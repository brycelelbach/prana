////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_A016B2C4_FCD9_43BF_8E47_6B6FE93073B4)
#define BSP_A016B2C4_FCD9_43BF_8E47_6B6FE93073B4

#include <boost/spirit/home/prana/phxpr/core/unary_function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct directory_predicate_function:
  unary_function<directory_predicate_function>
{
  directory_predicate_function (boost::shared_ptr<actor_list> const& elements):
    base_type(elements) { }

  typedef utree result_type;

  utree eval (utree const& subject) const;
};

struct directory_predicate_composite:
  unary_composite<directory_predicate_function> { };

extern directory_predicate_composite const is_dir;

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_A016B2C4_FCD9_43BF_8E47_6B6FE93073B4

