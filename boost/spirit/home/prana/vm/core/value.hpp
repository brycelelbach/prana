/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_CORE_VALUE_HPP)
#define BOOST_SPIRIT_PRANA_VM_CORE_VALUE_HPP

#include <boost/ref.hpp>

#include <boost/spirit/home/prana/vm/core/function.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct value_function: actor<value_function> {
  utree val;

  value_function (utree const& val): val(val) { }

  utree eval (scope const&) const {
    return utree(boost::ref(val));
  }
};

struct value {
  typedef function result_type;

  function operator() (utree const& val) const {
    return function(value_function(val));
  }
};

value const val = { };

inline function protect (function const& f) {
  return val(f.f);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_CORE_VALUE_HPP

