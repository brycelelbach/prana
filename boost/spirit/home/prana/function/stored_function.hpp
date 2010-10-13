/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_STORED_FUNCTION_HPP)
#define BOOST_SPIRIT_PRANA_STORED_FUNCTION_HPP

#include <boost/spirit/home/prana/function/function_base.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename F>
struct stored_function: public function_base {
 public:
  F f;

  stored_function (F f = F()): f(f) { }

  virtual ~stored_function (void) { }

  virtual utree operator() (scope const& env) const { return f(env); }
  
  virtual function_base* clone (void) const {
    return new stored_function<F>(*this);
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_STORED_FUNCTION_HPP
