/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SYMBOL_HPP)
#define BOOST_SPIRIT_PRANA_SYMBOL_HPP

#include <string>

#include <climits>
#include <cstring>
#include <cwchar>

#include <boost/cstdint.hpp>

#include <boost/spirit/home/prana/meta/unroll_copy.hpp>
#include <boost/spirit/home/prana/adt/range.hpp>

namespace boost {
namespace spirit {
namespace prana {

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

  static size_type const stack_size;

  void default_construct (void);
 
  void copy (symbol const&);
  void copy (Char const*);
  template<typename Iterator> void copy (Iterator, Iterator);
  template<typename Container> void copy (Container const&);
 
  static symbol make (void);
  template<typename Iterator> static symbol make (Iterator, Iterator);
  template<typename Container> static symbol make (Container const&);
  
  void free (void);
  
  template<typename Container> Container get (void) const;

  iterator begin (void) const;
  iterator end (void) const;

  template<typename Container> bool operator== (Container const&) const;
  template<typename Container> bool operator!= (Container const&) const;
  
  std::basic_string<Char> str (void) const;
  
  union {
    struct {
      uint8_t storage;
      Char str[(sizeof(void*[2]) / sizeof(Char)) - 1];
    } stack;
    struct {
      uint8_t storage;
      range<Char const*>* str;
    } heap;
  } data;
};

template<typename Char>
typename symbol<Char>::size_type const symbol<Char>::stack_size =
  (sizeof(void*[2]) / sizeof(Char)) - 1;

template<typename Char>
inline void symbol<Char>::default_construct (void) {
  data.stack.storage = 1;
  Char* ptr = data.stack.str;
  unroll_copy_c<stack_size, '\0'>::apply(ptr);
}

template<typename Char>
inline void symbol<Char>::copy (symbol const& other) {
  copy(other.begin(), other.end());
}

template<typename Char>
inline void symbol<Char>::copy (Char const* c) {
  copy(c, c + std::strlen(c));
}

template<>
inline void symbol<wchar_t>::copy (wchar_t const* c) {
  copy(c, c + std::wcslen(c));
}

template<typename Char>
template<typename Iterator>
inline void symbol<Char>::copy (Iterator f, Iterator l) {
  free();

  size_type size = l - f;

  if (!size) {
    data.stack.storage = CHAR_MAX;
    return;
  }

  else if (size <= stack_size) {
    data.stack.storage = size;
  
    for (size_type i = 0; i != size; ++i) data.stack.str[i] = *f++;
  }

  else { // store it in the heap
    data.stack.storage = 0;
  
    Char* p = new Char[size];
    for (size_type i = 0; i != size; ++i) p[i] = *f++;
  
    data.heap.str = new range<Char const*>;
    data.heap.str->default_construct();
    data.heap.str->copy(p, size); 
  }
}

template<typename Char>
template<typename Container>
inline void symbol<Char>::copy (Container const& c) {
  copy(c.begin(), c.end());
}

template<typename Char>
inline symbol<Char> symbol<Char>::make (void) {
  symbol s;
  s.default_construct();
  return s;
}

template<typename Char>
template<typename Iterator>
inline symbol<Char> symbol<Char>::make (Iterator f, Iterator l) {
  symbol s;
  s.copy(f, l);
  return s;
} 

template<typename Char>
template<typename Container>
inline symbol<Char> symbol<Char>::make (Container const& c) {
  copy(c.begin(), c.end());
}

template<typename Char>
inline void symbol<Char>::free (void) { 
  if (!data.stack.storage) {
    delete[] data.heap.str->begin();
    data.heap.str->free();
    delete data.heap.str;
    default_construct();
  } 
}

template<typename Char>
template<typename Container>
inline Container symbol<Char>::get (void) const {
  if (data.stack.storage == CHAR_MAX) return Container();
  return Container(begin(), end());
}

template<typename Char>
inline typename symbol<Char>::iterator symbol<Char>::begin (void) const {
  if (!data.stack.storage) return data.heap.str->begin();
  return data.stack.str;
}

template<typename Char>
inline typename symbol<Char>::iterator symbol<Char>::end (void) const {
  if (!data.stack.storage) return data.heap.str->end();
  return data.stack.str + data.stack.storage;
}

template<typename Char>
inline std::basic_string<Char> symbol<Char>::str (void) const {
  return get<std::basic_string<Char> >();
}

template<typename Char>
template<typename Container>
inline bool symbol<Char>::operator== (Container const& c) const {
  return std::equal(c.begin(), c.end(), str());
}

template<typename Char>
template<typename Container>
inline bool symbol<Char>::operator!= (Container const& c) const {
  return !std::equal(c.begin(), c.end(), str());
}

#undef STORAGE     
#undef HEAP       
#undef HEAP_SIZE   
#undef STACK_SIZE  

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SYMBOL_HPP
