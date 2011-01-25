/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_VM_ENVIRONMENT_FORWARD_HPP)
#define BSP_VM_ENVIRONMENT_FORWARD_HPP

#include <boost/function.hpp>

#include <boost/spirit/home/prana/vm/exception.hpp>
#include <boost/spirit/home/prana/vm/core/function.hpp>
#include <boost/spirit/home/prana/vm/environment/basic.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct forward_environment: basic_environment<forward_environment, function*> {
  forward_environment (forward_environment* parent = 0):
    forward_environment::base_type(parent) { }

  void define (key_type const& name, value_type const& val_) {
    if (definitions.find(name) != definitions.end())
      throw duplicate_identifier(name);

    definitions[name] = val_; 
  }

  result_type find (key_type const& name) {
    iterator i = definitions.find(name);

    if (i == definitions.end())
      return sentinel();
    else
      return i->second;
  }

  result_type sentinel (void) const {
    return 0; 
  }
};

} // prana
} // spirit
} // boost

#endif // BSP_VM_ENVIRONMENT_FORWARD_HPP

