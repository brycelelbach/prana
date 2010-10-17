/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_UNROLL_COPY_HPP)
#define BOOST_SPIRIT_PRANA_UNROLL_COPY_HPP

#include <cstddef>

#include <boost/config.hpp>

namespace boost {
namespace spirit {
namespace prana {

// big thanks to jjuran for suggesting this solution!

template<std::size_t N> struct unroll_copy {
  template<typename IteratorA, typename IteratorB>
  static inline void apply (IteratorA& a, IteratorB& b) {
    *b = *a;
    unroll_copy<N - 1>::apply(++a, ++b);
  }
};

template<> struct unroll_copy<0> {
  template<typename IteratorA, typename IteratorB>
  static inline void apply (IteratorA&, IteratorB&) {
    // end recursion
  }
};

template<std::size_t N, std::size_t C> struct unroll_copy_c {
  template<typename Iterator>
  static inline void apply (Iterator& it) {
    *it = C;
    unroll_copy_c<N - 1, C>::apply(++it);
  }
};

template<std::size_t C> struct unroll_copy_c<0, C> {
  template<typename Iterator>
  static inline void apply (Iterator&) {
    // end recursion
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_UNROLL_COPY_HPP
