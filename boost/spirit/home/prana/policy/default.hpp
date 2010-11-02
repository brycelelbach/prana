/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_POLICY_DEFAULT_HPP)
#define BOOST_SPIRIT_PRANA_POLICY_DEFAULT_HPP

#include <boost/spirit/home/prana/functional/copiers.hpp>
#include <boost/spirit/home/prana/functional/error_handlers.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace policy {

struct default_ {
  typedef char                          char_type;
  typedef functional::exception_handler error_handler;
  typedef functional::shallow_copier    copier;
};

} // policy
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_POLICY_DEFAULT_HPP
