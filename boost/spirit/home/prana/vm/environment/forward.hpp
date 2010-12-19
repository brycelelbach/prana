/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_FORWARD_HPP)
#define BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_FORWARD_HPP

#include <boost/function.hpp>

#include <boost/spirit/home/prana/vm/exception.hpp>
#include <boost/spirit/home/prana/vm/core/function.hpp>
#include <boost/spirit/home/prana/vm/environment/basic.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct forward_environment: basic_environment<forward_environment, function*> {
  forward_environment (forward_environment* parent = 0):
    basic_environment(parent) { }

  void define (std::string const& name, value_type const& val_) {
    if (definitions.find(name) != definitions.end())
      throw duplicate_identifier(name);

    definitions[name] = val_; 
  }

  value_type find (std::string const& name) {
    std::map<std::string, value_type>::iterator i = definitions.find(name);

    if (i == definitions.end())
      return 0;
    else
      return i->second;
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_ENVIRONMENT_FORWARD_HPP

