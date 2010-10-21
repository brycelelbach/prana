/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_EXCEPTIONS_HPP)
#define BOOST_SPIRIT_PRANA_EXCEPTIONS_HPP

#include <stdexcept>

namespace boost {
namespace spirit {
namespace prana {

template<typename Data>
struct hygienic_error: public std::runtime_error {
  template<typename T>
  hygienic_error (
    T const&, char const* message = "boost::spirit::prana::hygienic_error"
  );
  
  virtual ~hygienic_error (void) throw();

  Data context;
};

template<typename Data>
template<typename T>
hygienic_error<Data>::hygienic_error (T const& val, char const* message):
  std::runtime_error(message), context(val) {
    // line here to ease breakpointing in a debugger
  } 

template<typename Data>
hygienic_error<Data>::~hygienic_error (void) throw() {
  // line here to ease breakpointing in a debugger
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_EXCEPTIONS_HPP
