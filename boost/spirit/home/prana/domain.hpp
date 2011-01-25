/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_DOMAIN_HPP)
#define BSP_DOMAIN_HPP

#include <boost/spirit/home/support/unused.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct domain { };

struct unused_type: spirit::unused_type {
  struct type_definition;                            
  typedef std::size_t value_type;     
  BOOST_STATIC_CONSTANT(value_type, value = 0);      
  typedef void* data_type; 
};

struct sentinel_type: spirit::unused_type { };

static unused_type const unused = unused_type();

static sentinel_type const sentinel = sentinel_type();

} // prana
} // spirit
} // boost

#endif // BSP_DOMAIN_HPP

