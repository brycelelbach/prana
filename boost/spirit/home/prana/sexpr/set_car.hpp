/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_SET_CAR_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_SET_CAR_HPP

#include <boost/utility/enable_if.hpp>

#include <boost/spirit/home/prana/traits.hpp>
#include <boost/spirit/home/prana/sexpr/core.hpp>

namespace boost {
namespace spirit {
namespace prana {

/// R5RS section 6.3.2
/// 
/// procedure: set-car! pair obj
///   Stores obj in the car field of pair. The value returned by `set-car!' is
///   unspecified. 
///
/// Scheme:
///   (define (f) (list 'not-a-constant-list))
///   (set-car! (f) 3)

template<class X, class Y, class Dummy = void>
struct car_setter;

template<class X, class Y>
struct car_setter<X, Y,
  typename enable_if<is_interface_functor<Y>, void>::type
> {
  struct implementation_functor;

  template<class> struct result;

  template<class This>
  struct result<This(X&, Y const&)> {
    typedef X& type;
  };

  template<class TagX, class TagY>
  X& operator() (X&, Y const&) const;
};

template<class X, class Y>
struct car_setter<X, Y,
  typename disable_if<is_interface_functor<Y>, void>::type
> {
  struct implementation_functor;

  template<class> struct result;

  template<class This>
  struct result<This(X&, Y const&)> {
    typedef X& type;
  };

  template<class TagX, class TagY>
  X& operator() (X&, Y const&) const;
};

struct set_car {
  struct interface_functor;

  template<class> struct result;

  template<class This, class X, class Y>
  struct result<This(X&, Y const&)> {
    typedef X& type;
  };

  template<class X, class Y>
  X& operator() (X&, Y const&) const;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SEXPR_SET_CAR_HPP
