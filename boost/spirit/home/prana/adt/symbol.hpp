/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_SYMBOL_HPP)
#define BOOST_SPIRIT_PRANA_ADT_SYMBOL_HPP

#include <climits>
#include <cstring>
#include <cwchar>

#include <string>

#include <boost/cstdint.hpp>

#include <boost/spirit/home/prana/adt/range.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace adt {

template<typename Char>
struct symbol {
 public:
  typedef Char        value_type;
  typedef Char&       reference;
  typedef Char const& const_reference;
  typedef Char*       pointer;
  typedef Char const* const_pointer;
  typedef std::size_t size_type; 
  typedef Char const* iterator;
  typedef Char const* const_iterator;

  typedef range<iterator>       range_type;
  typedef range<const_iterator> const_range_type;

  static size_type const stack_size;

  void default_construct (void);
  
  void shallow_copy (symbol const&);
 
  void deep_copy (symbol const&);
  void deep_copy (Char const*);
  template<typename Iterator> void deep_copy (Iterator, Iterator);
  template<typename Container> void deep_copy (Container const&);
  
  void free (void);
  
  template<typename Container> Container get (void) const;

  iterator begin (void) const;
  iterator end (void) const;

  template<typename Container> bool operator== (Container const&) const;
  template<typename Container> bool operator!= (Container const&) const;
  
  std::basic_string<Char> str (void) const;
  
  union {
    struct {
      boost::uint8_t storage;
      Char str[(sizeof(void*[2]) / sizeof(Char)) - 1];
    } stack;
    struct {
      boost::uint8_t storage;
      boost::uint8_t alias;
      range_type* str;
    } heap;
  };
};

template<typename Char>
typename symbol<Char>::size_type const symbol<Char>::stack_size =
  (sizeof(void*[2]) / sizeof(Char)) - 1;

template<typename Char>
inline void symbol<Char>::default_construct (void) {
  stack.storage = 1;
  for (size_type i = 0; i < stack_size; ++i) stack.str[i] = '\0';
}

template<typename Char>
inline void symbol<Char>::shallow_copy (symbol const& other) {
  // EXPLAIN (wash): If the existing symbol is storing data on the heap, we
  // reuse the allocated range.
  if (other != *this) {
    if (stack.storage) {
      heap.str = new range_type;
      heap.str->default_construct();
    } else {
      delete[] heap.str->begin();
    }
    
    stack.storage = 0;
    heap.alias = 1;
    heap.str->deep_copy(other.begin(), other.end());
  }
}

template<typename Char>
inline void symbol<Char>::deep_copy (symbol const& other) {
  if (other != *this) deep_copy(other.begin(), other.end());
}

template<typename Char>
inline void symbol<Char>::deep_copy (Char const* c) {
  // DISCUSS (wash): Is this inefficient? Can we find a way to avoid using
  // strlen?
  deep_copy(c, c + std::strlen(c));
}

template<>
inline void symbol<wchar_t>::deep_copy (wchar_t const* c) {
  // DISCUSS (wash): Is there a way to negate the need for this specialization?
  deep_copy(c, c + std::wcslen(c));
}

template<typename Char>
template<typename Iterator>
inline void symbol<Char>::deep_copy (Iterator f, Iterator l) {
  free();

  size_type size = l - f;

  if (!size) {
    stack.storage = CHAR_MAX;
    return;
  }

  else if (size <= stack_size) {
    stack.storage = size;
  
    for (size_type i = 0; i != size; ++i) stack.str[i] = *f++;
  }

  else {
    stack.storage = 0;
  
    Char* p = new Char[size];
    for (size_type i = 0; i != size; ++i) p[i] = *f++;
  
    heap.str = new range_type;
    heap.str->default_construct();
    heap.str->deep_copy(p, size); 
  }
}

template<typename Char>
template<typename Container>
inline void symbol<Char>::deep_copy (Container const& c) {
  deep_copy(c.begin(), c.end());
}

template<typename Char>
inline void symbol<Char>::free (void) { 
  if (!stack.storage) {
    if (!heap.alias) delete[] heap.str->begin();
    heap.str->free();
    delete heap.str;
    stack.storage = CHAR_MAX;
  } 
}

template<typename Char>
template<typename Container>
inline Container symbol<Char>::get (void) const {
  if (stack.storage == CHAR_MAX) return Container();
  return Container(begin(), end());
}

template<typename Char>
inline typename symbol<Char>::iterator symbol<Char>::begin (void) const {
  if (!stack.storage) return heap.str->begin();
  return stack.str;
}

template<typename Char>
inline typename symbol<Char>::iterator symbol<Char>::end (void) const {
  if (!stack.storage) return heap.str->end();
  return stack.str + stack.storage;
}

template<typename Char>
inline std::basic_string<Char> symbol<Char>::str (void) const {
  return get<std::basic_string<Char> >();
}

template<typename Char>
template<typename Container>
inline bool symbol<Char>::operator== (Container const& c) const {
  // DISCUSS (wash): Is this too slow? Can we do this faster?
  return std::equal(c.begin(), c.end(), str());
}

template<typename Char>
template<typename Container>
inline bool symbol<Char>::operator!= (Container const& c) const {
  // DISCUSS (wash): Is this too slow? Can we do this faster?
  return !std::equal(c.begin(), c.end(), str());
}

} // adt
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_SYMBOL_HPP
