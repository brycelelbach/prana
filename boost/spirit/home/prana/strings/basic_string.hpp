/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_BASIC_STRING_HPP)
#define BOOST_SPIRIT_PRANA_BASIC_STRING_HPP

#include <cstddef>

#include <boost/spirit/home/prana/tree_type.hpp>

namespace boost {
namespace spirit {
namespace prana {

// A typed string with parametric Base storage. The storage can be any
// range or (stl container) of chars.
template<typename Base, tree_type::info type_>
struct basic_string: Base {
 public:
  static tree_type::info const type = type_;

  basic_string (void): Base() { }

  basic_string (Base const& base): Base(base) { }

  template<typename Iterator>
  basic_string (Iterator bits, std::size_t len): Base(bits, bits + len) { }

  template<typename Iterator>
  basic_string (Iterator first, Iterator last): Base(first, last) { }

  basic_string& operator= (basic_string const& other) {
    Base::operator=(other);
    return *this;
  }

  basic_string& operator= (Base const& other) {
    Base::operator=(other);
    return *this;
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_BASIC_STRING_HPP
