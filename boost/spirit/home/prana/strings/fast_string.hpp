/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_FAST_STRING_HPP)
#define BOOST_SPIRIT_PRANA_FAST_STRING_HPP

#include <boost/swap.hpp>

#include <boost/type_traits/alignment_of.hpp>

namespace boost {
namespace spirit {
namespace prana {

/*=============================================================================
fast_string<> is a primitive string implementation designed for internal use in
utree and linked_list<>. Originally, fast_string<> was not a template; it's
buffer size was linked to linked_list<> (which was also originally not a
template). I (wash) templatized fast_string<> during my effort to reduce
dependencies between utree utilities, thus making utree more modular.

fast_string<> is deliberately a POD to allow it to be placed in a union. This
structure utilizes (sizeof(List) * alignment_of(List)) / sizeof(char) In a 32
bit system, this is 14 bytes. The two extra bytes are used by utree to store
management info.

This is a const string (i.e. immutable). It stores the characters directly if
possible and only uses the heap if the string does not fit. Null characters are
allowed, making it suitable to encode raw binary. The string length is encoded
in the first byte if the string is placed in-situ, else, the length plus a
pointer to the string in the heap are stored.
=============================================================================*/

template<typename List>
class fast_string {
 public:
  typedef std::size_t size_type;
  
  BOOST_STATIC_CONSTANT(size_type, buff_size =
    (sizeof(List) + boost::alignment_of<List>::value) / sizeof(char));

  BOOST_STATIC_CONSTANT(size_type, small_string_size =
    buff_size - sizeof(char));

  BOOST_STATIC_CONSTANT(size_type, max_string_len =
    small_string_size - 1);

  struct heap_store {
    char* str;
    std::size_t size;
  };

 private:
  union {
    char buff[buff_size];
    heap_store heap;
  };

 public:
  #define INFO buff[small_string_size]

  int get_type (void) const {
    return INFO >> 1;
  }
  
  void set_type (int t) {
    INFO = (t << 1) | (INFO & 1);
  }

  bool is_heap_allocated (void) const {
    return INFO & 1;
  } 

  size_type size (void) const {
    if (INFO & 1) // aka is_heap_allocated()
      return heap.size;
    else
      return max_string_len - buff[small_string_size - 1];
  }

  char const* str (void) const {
    if (INFO & 1) // aka is_heap_allocated()
      return heap.str;
    else
      return buff;
  }

  template<typename Iterator>
  void construct (Iterator f, Iterator l) {
    size_type size = l - f;
    char* str = 0;

    if (size < small_string_size) {
      // if it fits, store it in-situ; small_string_size - length of the string
      // is placed in buff[small_string_size - 1]
      str = buff;
      buff[small_string_size - 1] = static_cast<char>(max_string_len - size);
      INFO &= ~0x1;
    }

    else { // else, store it in the heap
      str = new char[size + 1]; // add one for the null char
      heap.str = str;
      heap.size = size;
      INFO |= 0x1;
    }

    // FIXME: there's gotta be some way to unroll this with TMP
    for (std::size_t i = 0; i != size; ++i) *str++ = *f++;

    *str = '\0'; // add the null char
  }

  void swap (fast_string& other) {
    boost::swap(*this, other);
  }
  
  void free (void) { 
    if (heap.str && (INFO & 1)) {
      delete[] heap.str;
      heap.str = 0;
    } 
  }
  
  void copy (fast_string const& other) {
    construct(other.str(), other.str() + other.size());
  }

  char& info (void) {
    return buff[small_string_size];
  }
  
  char info (void) const {
    return buff[small_string_size];
  }

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
