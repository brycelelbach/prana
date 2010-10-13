/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_FAST_STRING_HPP)
#define BOOST_SPIRIT_PRANA_FAST_STRING_HPP

#include <boost/type_traits/alignment_of.hpp>

#include <boost/spirit/home/prana/adt/list_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

// keep as POD! do not implement dtor
struct fast_string {
 public:
  static std::size_t const buff_size =
    (sizeof(list) + boost::alignment_of<list>::value) / sizeof(char);

  static std::size_t const small_string_size =
    buff_size - sizeof(char);

  static std::size_t const max_string_len =
    small_string_size - 1;

  struct heap_store {
    char* str;
    std::size_t size;
  };

  union {
    char buff[buff_size];
    heap_store heap;
  };

  int get_type (void) const { return info() >> 1; }
  void set_type (int t) { info() = (t << 1) | (info() & 1); }
  bool is_heap_allocated (void) const { return info() & 1; } 

  std::size_t size (void) const {
    if (is_heap_allocated())
      return heap.size;
    else
      return max_string_len - buff[small_string_size - 1];
  }

  char const* str (void) const {
    if (is_heap_allocated())
      return heap.str;
    else
      return buff;
  }

  template<typename Iterator>
  void construct (Iterator f, Iterator l) {
    unsigned const size = l - f;
    char* str = 0;

    if (size < small_string_size) {
    // if it fits, store it in-situ; small_string_size minus the length
    // of the string is placed in buff[small_string_size - 1]
      str = buff;
      buff[small_string_size - 1] = static_cast<char>(max_string_len - size);
      info() &= ~0x1;
    }

    else {
      // else, store it in the heap
      str = new char[size + 1]; // add one for the null char
      heap.str = str;
      heap.size = size;
      info() |= 0x1;
    }

    for (std::size_t i = 0; i != size; ++i) *str++ = *f++;

    *str = '\0'; // add the null char
  }

  void swap (fast_string& other) { std::swap(*this, other); }
  
  void free (void) { 
    if (heap.str && is_heap_allocated()) {
      delete[] heap.str;
      heap.str = 0;
    } 
  }
  
  void copy (fast_string const& other) {
    construct(other.str(), other.str() + other.size());
  }

  char& info (void) { return buff[small_string_size]; }
  char info (void) const { return buff[small_string_size]; }

  short tag (void) const { 
    return (int(buff[small_string_size - 2]) << 8) + buff[small_string_size - 1];
  }

  void tag (short tag) { 
    buff[small_string_size - 2] = tag >> 8;
    buff[small_string_size - 1] = tag & 0xff;
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_FAST_STRING_HPP
