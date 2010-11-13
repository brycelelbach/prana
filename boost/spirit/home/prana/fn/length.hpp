/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_FN_LENGTH_HPP)
#define BOOST_SPIRIT_PRANA_FN_LENGTH_HPP

#include <cstddef> 

namespace boost {
namespace spirit {
namespace prana {

struct length {
  typedef std::size_t result_type;

  template<class Element>
  std::size_t operator() (Element const* e_) const {
    std::size_t result = 0;

    while (e_ && *e_) {
      ++result; ++e_;
    }

    return result;
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_FN_LENGTH_HPP
