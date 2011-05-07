/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010-2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_90B4660A_010E_487B_8272_FE832CD04AB7)
#define BSP_90B4660A_010E_487B_8272_FE832CD04AB7

#include <boost/ref.hpp>

#include <boost/spirit/home/prana/config.hpp>
#include <boost/spirit/home/prana/phxpr/core/function.hpp>
#include <boost/spirit/home/prana/phxpr/core/lambda.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {
    
struct procedure: composite<procedure> {
  // we must hold f by reference because functions can be recursive
  boost::reference_wrapper<function const> body;
  scope::size_type level;

  procedure (function const& body_, scope::size_type level_ = 0):
    body(body_), level(level_) { }

  function compose (boost::shared_ptr<actor_list> const& actors) const {
    return function(lambda_function(actors, body, level));
  }
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_90B4660A_010E_487B_8272_FE832CD04AB7
