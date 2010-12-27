/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_FUNCTION_FWD_HPP)
#define BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_FUNCTION_FWD_HPP

#include <boost/function.hpp>

#include <boost/spirit/home/prana/vm/core/function.hpp>
#include <boost/spirit/home/prana/vm/environment/basic.hpp>

namespace boost {
namespace spirit {
namespace prana {

typedef boost::function<function(actor_list const&)> compiled_function;

typedef fusion::vector3<compiled_function*, unsigned, bool> function_definition;

struct function_environment:
  basic_environment<
    function_environment, 
    fusion::vector3<compiled_function, unsigned, bool>,
    function_definition
  >
{
  function_environment (function_environment* parent = 0);
 
  template<typename Function>
  void define (key_type const& name, Function const& f, unsigned arity,
               bool fixed);

  result_type find (key_type const& name);

  result_type sentinel (void) const;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_FUNCTION_FWD_HPP

