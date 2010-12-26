/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SUPPORT_NO_OP_HPP)
#define BOOST_SPIRIT_PRANA_SUPPORT_NO_OP_HPP

#include <boost/spirit/home/prana/domain.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[unary_no_op
template<class X, class Dummy = void>
struct unary_no_op {
  typedef void result_type;

  void operator() (X const&) const { }
};
//]

//[binary_no_op
template<class X, class Y, class Dummy = void>
struct binary_no_op {
  typedef void result_type;

  void operator() (X const&, Y const&) const { }
};
//]

//[unary_no_op_with_return
template<class Result, class X, class Dummy = void>
struct unary_no_op_with_return {
  typedef Result result_type;

  Result operator() (X const&) const { return Result(); }
};
//]

//[binary_no_op_with_return
template<class Result, class X, class Y, class Dummy = void>
struct binary_no_op_with_return {
  typedef Result result_type;

  Result operator() (X const&, Y const&) const { return Result(); }
};
//]

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SUPPORT_NO_OP_HPP

