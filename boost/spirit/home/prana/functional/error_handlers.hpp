/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_FUNCTIONAL_ERROR_HANDLERS_HPP)
#define BOOST_SPIRIT_PRANA_FUNCTIONAL_ERROR_HANDLERS_HPP

#include <boost/assert.hpp>
#include <boost/spirit/home/prana/exceptions.hpp>

#if !defined(BOOST_NO_EXCEPTIONS)
  #include <boost/throw_exception.hpp>
#endif // BOOST_NO_EXCEPTIONS

namespace boost {
namespace spirit {
namespace prana {
namespace functional {

#if !defined(BOOST_NO_EXCEPTIONS)
  struct exception_handler {
    template<typename>
    struct result { typedef void type; };

    template<typename Exception>
    void operator() (Exception error) const;
  };

  template<typename Exception>
  inline void exception_handler::operator() (Exception error) const {
    boost::throw_exception(error);
  }
#endif // BOOST_NO_EXCEPTIONS

struct assertion_handler {
  template<typename>
  struct result { typedef void type; };

  template<typename Data>
  void operator() (hygienic_error<Data> error) const;
};

template<typename Data>
inline void assertion_handler::operator() (hygienic_error<Data> error) const {
  BOOST_ASSERT(0 && "boost::spirit::prana::hygienic_error");
}

} // functional
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_FUNCTIONAL_ERROR_HANDLERS_HPP
