/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INTEGRAL_FUNCTION_HPP)
#define BOOST_SPIRIT_PRANA_INTEGRAL_FUNCTION_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/type_traits/is_integral.hpp>

#include <boost/spirit/home/prana/utree.hpp>
#include <boost/spirit/home/prana/exceptions.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Base>
struct integral_function {
  typedef utree result_type;

  template<typename A, typename B>
  utree dispatch (A const&, B const&, boost::mpl::false_) const {
    throw illegal_integral_operation();
    return utree(); // cannot apply to non-integral types
  }

  template<typename A, typename B>
  utree dispatch (A const& a, B const& b, boost::mpl::true_) const {
    return Base::eval(a, b); // for integral types
  }

  // binary
  template<typename A, typename B>
  utree operator() (A const& a, B const& b) const {
    return dispatch(
      a, b,
      boost::mpl::and_<
        boost::is_integral<A>,
        boost::is_integral<B>
      >()
    );
  }

  template<typename A>
  utree dispatch (A const&, boost::mpl::false_) const {
    throw illegal_integral_operation();
    return utree(); // cannot apply to non-integral types
  }

  template<typename A>
  utree dispatch (A const& a, boost::mpl::true_) const {
    return Base::eval(a); // for integral types
  }

  // unary
  template<typename A>
  utree operator() (A const& a) const {
    return dispatch(a, boost::is_integral<A>());
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INTEGRAL_FUNCTION_HPP