/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_CLEAR_HPP)
#define BOOST_SPIRIT_PRANA_CLEAR_HPP

#include <boost/mpl/or.hpp>

#include <boost/utility/result_of.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/spirit/home/prana/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/sexpr/cast.hpp>
#include <boost/spirit/home/prana/sexpr/core.hpp>

namespace boost {
namespace spirit {
namespace prana {

namespace functor {

template<class TagX, class Dummy = prana::unused_type>
struct clear;

template<class TagX, class Dummy>
struct clear {
  typedef void result_type;
  
  template<class X>
  void operator() (X& x) const {
    x.car = 0;
  }
};

template<class TagX>
struct clear<
  TagX,
  typename enable_if<
    prana::is_heap_allocated_type<TagX>, prana::unused_type
  >::type
> {
  typedef void result_type;

  template<class X>
  void operator() (X& x) const {
    typename TagX::data_type* p = cast_car<TagX>(x);
    if (p && *p)
      delete *p;
  }
};

} // functor

template<class X>
inline void clear (X& x) {
  return dispatch<typename X::registry, functor::clear, X>(x);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_CLEAR_HPP

