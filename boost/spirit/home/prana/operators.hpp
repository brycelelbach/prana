
/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_OPERATORS_HPP)
#define BOOST_SPIRIT_PRANA_OPERATORS_HPP

#include <boost/spirit/home/prana/operators/arithmetic_function.hpp>
#include <boost/spirit/home/prana/operators/integral_function.hpp>
#include <boost/spirit/home/prana/operators/logical_function.hpp>
#include <boost/spirit/home/prana/operators/tree_equal.hpp>
#include <boost/spirit/home/prana/operators/tree_less_than.hpp>
#include <boost/spirit/home/prana/operators/tree_print.hpp>

namespace boost {
namespace spirit {
namespace prana {

#define BOOST_SPIRIT_CREATE_FUNCTION(name, expr, base)                  \
  struct BOOST_PP_CAT(function_impl_, name) {                           \
    template<typename A, typename B>                                    \
    static utree eval (A const& a, B const& b) { return utree(expr); }  \
                                                                        \
    template<typename A>                                                \
    static utree eval (A const& a) {                                    \
      static int b;                                                     \
      (void) b;                                                         \
      return utree(expr);                                               \
    }                                                                   \
  };                                                                    \
                                                                        \
  base<BOOST_PP_CAT(function_impl_, name)> const                        \
    BOOST_PP_CAT(base, BOOST_PP_CAT(_, name)) = {};                     \
  /**/

#define BOOST_SPIRIT_CREATE_ARITHMETIC_FUNCTION(name, expr)             \
  BOOST_SPIRIT_CREATE_FUNCTION(name, expr, arithmetic_function)         \
  /**/

#define BOOST_SPIRIT_CREATE_INTEGRAL_FUNCTION(name, expr)               \
  BOOST_SPIRIT_CREATE_FUNCTION(name, expr, integral_function)           \
  /**/

#define BOOST_SPIRIT_CREATE_LOGICAL_FUNCTION(name, expr)                \
  BOOST_SPIRIT_CREATE_FUNCTION(name, expr, logical_function)            \
  /**/

inline bool operator== (utree const& a, utree const& b) {
  return visit(a, b, tree_equal());
}

inline bool operator< (utree const& a, utree const& b) {
  return visit(a, b, tree_less_than());
}

inline bool operator!= (utree const& a, utree const& b) {
  return !(a == b);
}

inline bool operator> (utree const& a, utree const& b) {
  return b < a;
}

inline bool operator<= (utree const& a, utree const& b) {
  return !(b < a);
}

inline bool operator>= (utree const& a, utree const& b) {
  return !(a < b);
}

inline std::ostream& operator<< (std::ostream& out, utree const& x) {
  visit(x, tree_print(out));
  return out;
}

inline std::ostream& operator<< (std::ostream& out, nil const& x) {
  return out;
}

BOOST_SPIRIT_CREATE_LOGICAL_FUNCTION(and_, a&&b);
BOOST_SPIRIT_CREATE_LOGICAL_FUNCTION(or_, a||b);
BOOST_SPIRIT_CREATE_LOGICAL_FUNCTION(not_, !a);

BOOST_SPIRIT_CREATE_ARITHMETIC_FUNCTION(plus, a+b);
BOOST_SPIRIT_CREATE_ARITHMETIC_FUNCTION(minus, a-b);
BOOST_SPIRIT_CREATE_ARITHMETIC_FUNCTION(times, a*b);
BOOST_SPIRIT_CREATE_ARITHMETIC_FUNCTION(divides, a/b);
BOOST_SPIRIT_CREATE_INTEGRAL_FUNCTION(modulus, a%b);
BOOST_SPIRIT_CREATE_ARITHMETIC_FUNCTION(negate, -a);

BOOST_SPIRIT_CREATE_INTEGRAL_FUNCTION(bitand_, a&b);
BOOST_SPIRIT_CREATE_INTEGRAL_FUNCTION(bitor_, a|b);
BOOST_SPIRIT_CREATE_INTEGRAL_FUNCTION(bitxor_, a^b);
BOOST_SPIRIT_CREATE_INTEGRAL_FUNCTION(shift_left, a<<b);
BOOST_SPIRIT_CREATE_INTEGRAL_FUNCTION(shift_right, a>>b);
BOOST_SPIRIT_CREATE_INTEGRAL_FUNCTION(invert, ~a);

inline utree operator&& (utree const& a, utree const& b) {
  return visit(a, b, logical_function_and_);
}

inline utree operator|| (utree const& a, utree const& b) {
  return visit(a, b, logical_function_or_);
}

inline utree operator! (utree const& a) {
  return visit(a, logical_function_not_);
}

inline utree operator+ (utree const& a, utree const& b) {
  return visit(a, b, arithmetic_function_plus);
}

inline utree operator- (utree const& a, utree const& b) {
  return visit(a, b, arithmetic_function_minus);
}

inline utree operator* (utree const& a, utree const& b) {
  return visit(a, b, arithmetic_function_times);
}

inline utree operator/ (utree const& a, utree const& b) {
  return visit(a, b, arithmetic_function_divides);
}

inline utree operator% (utree const& a, utree const& b) {
  return visit(a, b, integral_function_modulus);
}

inline utree operator- (utree const& a) {
  return visit(a, arithmetic_function_negate);
}

inline utree operator& (utree const& a, utree const& b) {
  return visit (a, b, integral_function_bitand_);
}

inline utree operator| (utree const& a, utree const& b) {
  return visit(a, b, integral_function_bitor_);
}

inline utree operator^ (utree const& a, utree const& b) {
  return visit(a, b, integral_function_bitxor_);
}

inline utree operator<< (utree const& a, utree const& b) {
  return visit(a, b, integral_function_shift_left);
}

inline utree operator>> (utree const& a, utree const& b) {
  return visit(a, b, integral_function_shift_right);
}

inline utree operator~ (utree const& a) {
  return visit(a, integral_function_invert);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_OPERATORS_HPP
