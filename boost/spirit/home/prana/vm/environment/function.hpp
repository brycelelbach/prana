/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_FUNCTION_HPP)
#define BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_FUNCTION_HPP

#include <boost/function.hpp>

#include <boost/fusion/include/at_c.hpp>

#include <boost/spirit/home/prana/vm/exception.hpp>
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
  function_environment (function_environment* parent = 0):
    basic_environment(parent) { }
 
  template<typename Function>
  void define (key_type const& name, Function const& f, unsigned arity,
               bool fixed)
  {
    if (definitions.find(name) != definitions.end())
      throw duplicate_identifier(name);

    definitions[name] = value_type(compiled_function(f), arity, fixed);
  }

  result_type find (key_type const& name) {
    using fusion::at_c;

    iterator i = definitions.find(name);

    if (i != definitions.end())
      return result_type(&at_c<0>(i->second),
                         at_c<1>(i->second),
                         at_c<2>(i->second));
    else if (outer)
      return (*outer)(name);

    return sentinel();
  }

  result_type sentinel (void) const {
    return result_type((compiled_function*)0, 0, false);
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_FUNCTION_HPP

