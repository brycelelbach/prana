/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_LIGHT_BIND_HPP)
#define BOOST_SPIRIT_PRANA_LIGHT_BIND_HPP

#include <boost/utility/result_of.hpp>

namespace boost {
namespace spirit {
namespace prana {

/* EXPLAIN (djowel): Simple binder for binary visitation (we don't want to bring
   in the big guns). */
template<class F, class X, class TagX>
class dispatch_binder {
 private:
  F f;
  X& x; /* EXPLAIN (djowel): Always by reference. */
  TagX tagx; /* EXPLAIN (wash): This is constructed in place in the trampoline,
                and should be small enough to be based by value. */

 public:
  template<class> struct result;

  template<class This, class Y, class TagY>
  struct result<This(Y&, TagY)> {
    typedef typename result_of<
      F(X&, TagX, Y&, TagY)
    >::type type;
  };
  
  template<class This, class Y, class TagY>
  struct result<This(Y const&, TagY)> {
    typedef typename result_of<
      F(X&, TagX, Y const&, TagY)
    >::type type;
  };

  dispatch_binder (F f_, X& x_, TagX tagx_): f(f_), x(x_), tagx(tagx_) { }

  template<typename Y, typename TagY>
  typename result_of<
    F(X&, TagX, Y&, TagY)
  >::type operator() (Y& y, TagY tagy) const {
    return f(x, tagx, y, tagy);
  }

  template<typename Y, typename TagY>
  typename result_of<
    F(X&, TagX, Y const&, TagY)
  >::type operator() (Y const& y, TagY tagy) const {
    return f(x, tagx, y, tagy);
  }
};

template<typename F, typename X, typename TagX>
dispatch_binder<F, X const> dispatch_bind(F f, X const& x, TagX tagx) {
  return dispatch_binder<F, X const, TagX>(f, x, tagx);
}

template<typename F, typename X, typename TagX>
dispatch_binder<F, X> dispatch_bind(F f, X& x, TagX tagx) {
  return dispatch_binder<F, X, TagX>(f, x, tagx);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_LIGHT_BIND_HPP
