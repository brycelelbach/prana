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

/* EXPLAIN (wash): Tag binder for binding a templated operator() */
template<unsigned arity, class F, class TagX, class TagY = TagX>
class tag_binder;

template<class F, class TagX, class TagY>
class tag_binder<1, F, TagX, TagY> {
 private:
  F f;

 public:
  typedef typename F::result_type result_type;

  tag_binder (F f_): f(f_) { }

  template<typename X>
  typename F::result_type operator() (X& x) const {
    return f.template operator()<TagX, TagY>(x);
  }

  template<typename X>
  typename F::result_type operator() (X const& x) const {
    return f.template operator()<TagX, TagY>(x);
  }
};

template<class F, class TagX>
class tag_binder<1, F, TagX, TagX> {
 private:
  F f;

 public:
  typedef typename F::result_type result_type;

  tag_binder (F f_): f(f_) { }

  template<typename X>
  typename F::result_type operator() (X& x) const {
    return f.template operator()<TagX>(x);
  }

  template<typename X>
  typename F::result_type operator() (X const& x) const {
    return f.template operator()<TagX>(x);
  }
};

template<class F, class TagX, class TagY>
class tag_binder<2, F, TagX, TagY> {
 private:
  F f;

 public:
  typedef typename F::result_type result_type;

  tag_binder (F f_): f(f_) { }

  template<typename X, typename Y>
  typename F::result_type operator() (X& x, Y& y) const {
    return f.template operator()<TagX, TagY>(x, y);
  }

  template<typename X, typename Y>
  typename F::result_type operator() (X& x, Y const& y) const {
    return f.template operator()<TagX, TagY>(x, y);
  }
  
  template<typename X, typename Y>
  typename F::result_type operator() (X const& x, Y& y) const {
    return f.template operator()<TagX, TagY>(x, y);
  }

  template<typename X, typename Y>
  typename F::result_type operator() (X const& x, Y const& y) const {
    return f.template operator()<TagX, TagY>(x, y);
  }
};

template<class F, class TagX>
class tag_binder<2, F, TagX, TagX> {
 private:
  F f;

 public:
  typedef typename F::result_type result_type;

  tag_binder (F f_): f(f_) { }

  template<typename X, typename Y>
  typename F::result_type operator() (X& x, Y& y) const {
    return f.template operator()<TagX>(x, y);
  }

  template<typename X, typename Y>
  typename F::result_type operator() (X& x, Y const& y) const {
    return f.template operator()<TagX>(x, y);
  }
  
  template<typename X, typename Y>
  typename F::result_type operator() (X const& x, Y& y) const {
    return f.template operator()<TagX>(x, y);
  }

  template<typename X, typename Y>
  typename F::result_type operator() (X const& x, Y const& y) const {
    return f.template operator()<TagX>(x, y);
  }
};

/* EXPLAIN (djowel): Simple binder for binary visitation (we don't want to bring
   in the big guns). */
template<class F, class X>
class lhs_binder {
 private:
  F f;
  X& x; /* EXPLAIN (djowel): Always by reference. */

 public:
  typedef typename F::result_type result_type;

  lhs_binder (F f_, X& x_): f(f_), x(x_) { }
  
  template<typename Y>
  typename F::result_type operator() (Y& y) const {
    return f(x, y);
  }

  template<typename Y>
  typename F::result_type operator() (Y const& y) const {
    return f(x, y);
  }
};

template<class F, class Y>
class rhs_binder {
 private:
  F f;
  Y& y; 

 public:
  typedef typename F::result_type result_type;

  rhs_binder (F f_, Y& y_): f(f_), y(y_) { }
  
  template<typename X>
  typename F::result_type operator() (X& x) const {
    return f(x, y);
  }

  template<typename X>
  typename F::result_type operator() (X const& x) const {
    return f(x, y);
  }
};

/* EXPLAIN (wash): unary_tag_bind binds an unary operator() method with one or
   two template parameters; the resulting binder can be invoked as F(X) */

/* F<TagX>(X) -> F(X) */
template<typename TagX, typename F>
tag_binder<1, F, TagX> unary_tag_bind (F f) {
  return tag_binder<1, F, TagX>(f);
}

/* F<TagX, TagY>(X) -> F(X) */
template<typename TagX, typename TagY, typename F>
tag_binder<1, F, TagX, TagY> unary_tag_bind (F f) {
  return tag_binder<1, F, TagX, TagY>(f);
}

/* EXPLAIN (wash): binary_tag_bind works like unary_tag_bind; the only
   difference is the function call arity of the operator() bound. Invocation
   of the produced binder is F(X, Y). */

/* F<TagX>(X, Y) -> F(X, Y) */
template<typename TagX, typename F>
tag_binder<2, F, TagX> binary_tag_bind (F f) {
  return tag_binder<2, F, TagX>(f);
}

/* F<TagX, TagY>(X, Y) -> F(X, Y) */
template<typename TagX, typename TagY, typename F>
tag_binder<2, F, TagX, TagY> binary_tag_bind (F f) {
  return tag_binder<2, F, TagX, TagY>(f);
}

/* EXPLAIN (wash): lhs_bind binds binary functions; both arguments and
   template parameters can be bound. The return value is a binder that can be
   invoked with F(Y). */

/* F(X, Y) -> F(Y) */
template<typename F, typename X>
lhs_binder<F, X const> lhs_bind(F f, X const& x) {
  return lhs_binder<F, X const>(f, x);
}

/* F(X, Y) -> F(Y) */
template<typename F, typename X>
lhs_binder<F, X> lhs_bind(F f, X& x) {
  return lhs_binder<F, X>(f, x);
}

/* F<TagX>(X, Y) -> F(Y) */
template<typename TagX, typename F, typename X>
lhs_binder<F, X const> lhs_bind(F f, X const& x) {
  return lhs_binder<F, X const>(binary_tag_bind<TagX>(f), x);
}

/* F<TagX>(X, Y) -> F(Y) */
template<typename TagX, typename F, typename X>
lhs_binder<F, X> lhs_bind(F f, X& x) {
  return lhs_binder<F, X>(binary_tag_bind<TagX>(f), x);
}

/* F<TagX, TagY>(X, Y) -> F(Y) */
template<typename TagX, typename TagY, typename F, typename X>
lhs_binder<F, X const> lhs_bind(F f, X const& x) {
  return lhs_binder<F, X const>(binary_tag_bind<TagX, TagY>(f), x);
}

/* F<TagX, TagY>(X, Y) -> F(Y) */
template<typename TagX, typename TagY, typename F, typename X>
lhs_binder<F, X> lhs_bind(F f, X& x) {
  return lhs_binder<F, X>(binary_tag_bind<TagX, TagY>(f), x);
}

/* EXPLAIN (wash): rhs_bind binds the right hand side, aka the second parameter,
   of an operator() method. Template parameters can be bound as well. The
   resulting binder can be invoked with F(X). */

/* F(X, Y) -> F(X) */
template<typename F, typename Y>
rhs_binder<F, Y const> rhs_bind(F f, Y const& y) {
  return rhs_binder<F, Y const>(f, y);
}

/* F(X, Y) -> F(X) */
template<typename F, typename Y>
rhs_binder<F, Y> rhs_bind(F f, Y& y) {
  return rhs_binder<F, Y>(f, y);
}

/* F<TagX>(X, Y) -> F(X) */
template<typename TagX, typename F, typename Y>
rhs_binder<F, Y const> rhs_bind(F f, Y const& y) {
  return rhs_binder<F, Y const>(binary_tag_bind<TagX>(f), y);
}

/* F<TagX>(X, Y) -> F(X) */
template<typename TagX, typename F, typename Y>
rhs_binder<F, Y> rhs_bind(F f, Y& y) {
  return rhs_binder<F, Y>(binary_tag_bind<TagX>(f), y);
}

/* F<TagX, TagY>(X, Y) -> F(X) */
template<typename TagX, typename TagY, typename F, typename Y>
rhs_binder<F, Y const> rhs_bind(F f, Y const& y) {
  return rhs_binder<F, Y const>(binary_tag_bind<TagX, TagY>(f), y);
}

/* F<TagX, TagY>(X, Y) -> F(X) */
template<typename TagX, typename TagY, typename F, typename Y>
rhs_binder<F, Y> rhs_bind(F f, Y& y) {
  return rhs_binder<F, Y>(binary_tag_bind<TagX, TagY>(f), y);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_BIND_HPP
