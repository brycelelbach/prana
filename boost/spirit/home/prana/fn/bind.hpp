/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_BIND_HPP)
#define BOOST_SPIRIT_PRANA_BIND_HPP

namespace boost {
namespace spirit {
namespace prana {

// EXPLAIN (djowel): Simple binder for binary visitation (we don't want to bring
// in the big guns).
template<typename F, typename X>
struct binder {
 private:
  X& _x; // EXPLAIN (djowel): Always by reference.
  F _f;

 public:
  typedef typename F::result_type result_type;

  binder (F f_, X& x_): _x(x_), _f(f_) { }

  template<typename Y>
  typename F::result_type operator() (Y& y_) const {
    return f(_x, y_);
  }

  template<typename Y>
  typename F::result_type operator() (Y const& y_) const {
    return f(_x, y_);
  }
};

template<typename F, typename X>
binder<F, X const> bind(F f_, X const& x_) {
  return binder<F, X const>(f_, x_);
}

template<typename F, typename X>
binder<F, X> bind(F f_, X& x_) {
  return binder<F, X>(f_, x_);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_BIND_HPP
