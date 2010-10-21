/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_FUNCTIONAL_DEFAULT_THROWER_HPP)
#define BOOST_SPIRIT_PRANA_FUNCTIONAL_DEFAULT_THROWER_HPP

#include <boost/throw_exception.hpp>

#include <boost/spirit/home/prana/exceptions.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace functional {

struct default_thrower {
  template<typename>
  struct result { typedef void type; };

  template<typename Exception>
  void operator() (Exception error) const;
};

template<typename Exception>
void default_thrower::operator() (Exception error) const {
  boost::throw_exception(error);
}

} // functional
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_FUNCTIONAL_DEFAULT_THROWER_HPP
