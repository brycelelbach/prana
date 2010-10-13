/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_LOGICAL_FUNCTION_HPP)
#define BOOST_SPIRIT_PRANA_LOGICAL_FUNCTION_HPP

#include <boost/spirit/home/prana/utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Base>
struct logical_function {
  typedef utree result_type;

  // binary
  utree operator() (bool a, bool b) const {
    return Base::eval(a, b); // for boolean types
  }

  // binary
  template<typename A>
  utree operator() (A const& a, bool b) const {
    return Base::eval(true, b);
  }

  // binary
  template<typename B>
  utree operator() (bool a, B const& b) const {
    return Base::eval(a, true);
  }

  // binary
  template<typename A, typename B>
  utree operator() (A const& a, B const& b) const {
    return Base::eval(true, true);
  }

  // unary
  utree operator() (bool a) const {
    return Base::eval(a);
  }

  // unary
  template<typename A>
  utree operator() (A const& a) const {
    return Base::eval(true);
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_LOGICAL_FUNCTION_HPP
