/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_VM_CORE_EXTERNAL_FUNCTION_HPP)
#define BSP_VM_CORE_EXTERNAL_FUNCTION_HPP

#include <boost/spirit/home/prana/vm/core/lambda.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct external_function: composite<external_function> {
  // We must hold f by reference because functions can be recursive.
  reference_wrapper<function const> f;
  unsigned level;

  external_function (function const& f, unsigned level): f(f), level(level) { }

  using base_type::operator();

  function operator() (actor_list const& elements) const {
    return function(lambda_function(f, elements, level));
  }
};

} // prana
} // spirit
} // boost

#endif // BSP_VM_CORE_EXTERNAL_FUNCTION_HPP

