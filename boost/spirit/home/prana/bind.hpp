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

// simple binder for binary visitation (we don't want to bring in the big guns)
template<typename F, typename X>
struct binder {
 private:
  X& x; // always by reference
  F f;

 public:
  typedef typename F::result_type result_type;

  binder (F f, X& x): x(x), f(f) { }

  template<typename Y>
  typename F::result_type operator() (Y& y) const { return f(x, y); }

  template<typename Y>
  typename F::result_type operator() (Y const& y) const { return f(x, y); }
};

template<typename F, typename X>
binder<F, X const> bind(F f, X const& x) { return binder<F, X const>(f, x); }

template<typename F, typename X>
binder<F, X> bind(F f, X& x) { return binder<F, X>(f, x); }

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_BIND_HPP
