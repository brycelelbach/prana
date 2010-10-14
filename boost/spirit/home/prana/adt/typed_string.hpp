/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TYPED_STRING_HPP)
#define BOOST_SPIRIT_PRANA_TYPED_STRING_HPP

#include <cstddef>

#include <boost/spirit/home/prana/tree_type.hpp>
#include <boost/spirit/home/prana/adt/irange.hpp>

namespace boost {
namespace spirit {
namespace prana {

/*=============================================================================
A typed string with parametric Base storage. The storage can be any range or
(STL container) of chars.
=============================================================================*/

template<typename Base, tree_type::info type_>
struct typed_string: Base {
 public:
  static tree_type::info const type = type_;

  typed_string (void) { }

  typed_string (Base const& base): Base(base) { }

  template<typename Iterator>
  typed_string (Iterator bits, std::size_t len) {
    construct(bits, bits + len);
  }

  template<typename Iterator>
  typed_string (Iterator first, Iterator last) {
    construct(first, last);
  }

  typed_string& operator= (typed_string const& other) {
    Base::operator=(other);
    return *this;
  }

  typed_string& operator= (Base const& other) {
    Base::operator=(other);
    return *this;
  }
};

typedef typed_string<irange<char const*>, tree_type::binary_type>
  binary_range;

typedef typed_string<std::string, tree_type::binary_type>
  binary_string;

typedef typed_string<irange<char const*>, tree_type::string_type>
  utf8_string_range;

typedef typed_string<std::string, tree_type::string_type>
  utf8_string;

typedef typed_string<irange<char const*>, tree_type::symbol_type>
  utf8_symbol_range;

typedef typed_string<std::string, tree_type::symbol_type>
  utf8_symbol;

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TYPED_STRING_HPP
