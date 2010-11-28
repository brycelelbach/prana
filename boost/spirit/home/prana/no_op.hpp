/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_NOOP_HPP)
#define BOOST_SPIRIT_PRANA_NOOP_HPP

#include <boost/spirit/home/prana/domain.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class X, class Dummy = prana::unused>
struct unary_noop {
  typedef void result_type;

  void operator() (X&) const { }
};

template<class X, class Y, class Dummy = prana::unused>
struct binary_noop {
  typedef void result_type;

  void operator() (X&, Y&) const { }
};

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_NOOP_HPP ->*/
