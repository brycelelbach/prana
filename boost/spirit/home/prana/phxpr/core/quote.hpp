/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_CORE_QUOTE_HPP)
#define BSP_PHXPR_CORE_QUOTE_HPP

#include <boost/spirit/home/prana/config.hpp>

#include <boost/ref.hpp>

#include <boost/spirit/home/prana/phxpr/core/function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct quote_function: actor<quote_function> {
  utree val;

  quote_function (utree const& val_): val(val_) { }

  utree eval (scope const&) const {
    return utree(boost::ref(val));
  }
  
  utree clone (void) const {
    return utree(stored_function<quote_function>(quote_function(val)));
  }
};

struct quote_type {
  typedef function result_type;

  function operator() (utree const& val) const {
    return function(quote_function(val));
  }
  
  function operator() (function const& f) const {
    return function(quote_function(f.f));
  }
};

quote_type const quote = { };

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_CORE_QUOTE_HPP

