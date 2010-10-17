/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SYMBOL_HPP)
#define BOOST_SPIRIT_PRANA_SYMBOL_HPP

#include <cstring>
#include <climits>

#include <algorithm>

#include <boost/config.hpp>
#include <boost/integer.hpp>
#include <boost/static_assert.hpp>
#include <boost/detail/iterator.hpp>
#include <boost/type_traits/alignment_of.hpp>

#include <boost/spirit/home/prana/kind.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Char>
struct symbol_traits;

template<>
struct symbol_traits<char> {
  typedef boost::uint_t<sizeof(void*) * CHAR_BIT>::exact size_type;
  static size_type const stack_size;
  static size_type const meta_size;
  
  static inline size_type length (char const* s) {
    return std::strlen(s);
  }
};

symbol_traits<char>::size_type const symbol_traits<char>::stack_size =
  sizeof(void*[2]) / sizeof(char);

symbol_traits<char>::size_type const symbol_traits<char>::meta_size =
  sizeof(void*) / sizeof(char);

template<>
struct symbol_traits<wchar_t> {
  typedef boost::uint_t<sizeof(void*) * CHAR_BIT>::exact size_type;
  static size_type const stack_size;
  static size_type const meta_size;

  static inline size_type length (wchar_t const* s) {
    return std::wcslen(s);
  }
};

symbol_traits<wchar_t>::size_type const symbol_traits<wchar_t>::stack_size =
  sizeof(void*[2]) / sizeof(wchar_t);

symbol_traits<wchar_t>::size_type const symbol_traits<wchar_t>::meta_size =
  sizeof(void*) / sizeof(wchar_t);

template<typename Char>
struct symbol {
 public:
  typedef symbol_traits<Char> traits;
 
  typedef Char                       value_type;
  typedef Char&                      reference;
  typedef Char const&                const_reference;
  typedef Char*                      pointer;
  typedef Char const*                const_pointer;
  typedef typename traits::size_type size_type; 
  typedef Char const*                iterator;
  typedef Char const*                const_iterator;

  struct stack_store {
    Char      str[traits::stack_size];
    char      meta[traits::meta_size];
  };
    
  struct heap_store {
    Char*     str;
    size_type size;
  };

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
  
  // alias for begin
  std::basic_string<Char> str (void) const;
  
  template<size_type N>
  char meta (void) const;  
  
  template<size_type N>
  void meta (char);

  size_type size (void) const;
  
  union {
    stack_store stack;
    heap_store  heap;
  };
};

#define STORAGE     stack.meta[0]
#define HEAP        (1 << (CHAR_BIT - 1))
#define HEAP_SIZE   heap.size
#define STACK_SIZE  stack.meta[0]

template<typename Char>
inline void symbol<Char>::default_construct (void) {
  heap.str = 0;
  heap.size = 0;

  // FIXME: there's gotta be some way to unroll this with TMP
  for (size_type i = 0; i != traits::meta_size; ++i) stack.meta[i] = 0;
}

template<typename Char>
inline void symbol<Char>::copy (symbol const& other) {
  copy(other.str(), other.str() + other.size());
}

template<typename Char>
inline void symbol<Char>::copy (Char const* c) {
  // FIXME: this can be done inline, which will be faster, and
  // eliminate the need for length in traits
  copy(c, c + traits::length(c));
}

template<typename Char>
template<typename Iterator>
inline void symbol<Char>::copy (Iterator f, Iterator l) {
  free();

  size_type size = l - f;
  Char* str = 0;

  if (size <= traits::stack_size) {
    str = stack.str;
    STACK_SIZE = size;
  }

  else { // store it in the heap
    str = new Char[size];
    heap.str = str;
    heap.size = size;
    STORAGE = HEAP;
  }

  // FIXME: there's gotta be some way to unroll this with TMP
  for (size_type i = 0; i != size; ++i) *str++ = *f++;
}

template<typename Char>
template<typename Container>
inline void symbol<Char>::copy (Container const& c) {
  copy(c, begin(), c.end());
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
  copy(c, begin(), c.end());
}

template<typename Char>
inline void symbol<Char>::free (void) { 
  if (heap.str && (STORAGE & HEAP)) {
    delete[] heap.str;
    heap.str = 0;
    STORAGE = 0;
  } 
}

template<typename Char>
template<typename Container>
inline Container symbol<Char>::get (void) const {
  return Container(begin(), end());
}

template<typename Char>
inline typename symbol<Char>::iterator symbol<Char>::begin (void) const {
  if (STORAGE & HEAP) return heap.str;
  return stack.str;
}

template<typename Char>
inline typename symbol<Char>::iterator symbol<Char>::end (void) const {
  if (STORAGE & HEAP) return heap.str + size();
  return stack.str + size();
}

template<typename Char>
inline std::basic_string<Char> symbol<Char>::str (void) const {
  return std::basic_string<Char>(begin(), end());
}

template<typename Char>
template<typename symbol<Char>::size_type N>
inline char symbol<Char>::meta (void) const {
  BOOST_STATIC_ASSERT((N > 1) && (N <= symbol_traits<Char>::meta_size));
  return stack.meta[N];
}

template<typename Char>
template<typename symbol<Char>::size_type N>
inline void symbol<Char>::meta (char t) {
  BOOST_STATIC_ASSERT((N > 1) && (N <= symbol_traits<Char>::meta_size));
  stack.meta[N] = t;
}

template<typename Char>
inline typename symbol<Char>::size_type symbol<Char>::size (void) const {
  if (STORAGE & HEAP) return HEAP_SIZE;
  return STACK_SIZE;
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
