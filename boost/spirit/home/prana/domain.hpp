/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_DOMAIN_HPP)
#define BOOST_SPIRIT_PRANA_DOMAIN_HPP

#include <boost/spirit/home/support/unused.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct domain { };

//[ unused_type 
struct unused_type: spirit::unused_type /*< Derived from Boost.Fusion's
                                            unused_type, which is typedef'd in
                                            Spirit. >*/ {
  struct type_definition;                            
  typedef std::size_t value_type;     
  BOOST_STATIC_CONSTANT(value_type, value = 0);      
  typedef void* data_type; 
};
//]

//[ nil_type
struct nil_type: prana::unused_type { };
//]

//[ empty_list_type
struct empty_list_type: prana::unused_type { };
//]

static unused_type const unused = unused_type();

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_DOMAIN_HPP

