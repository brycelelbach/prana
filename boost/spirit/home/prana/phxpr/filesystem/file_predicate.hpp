////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_6392F984_84EA_4566_9166_77F44BB9089A)
#define BSP_6392F984_84EA_4566_9166_77F44BB9089A

#include <boost/spirit/home/prana/phxpr/core/unary_function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct file_predicate_function: unary_function<file_predicate_function> {
  file_predicate_function (boost::shared_ptr<actor_list> const& elements):
    base_type(elements) { }

  typedef utree result_type;

  utree eval (utree const& subject) const;
};

struct file_predicate_composite: unary_composite<file_predicate_function> { };

extern file_predicate_composite const is_file;

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_6392F984_84EA_4566_9166_77F44BB9089A

