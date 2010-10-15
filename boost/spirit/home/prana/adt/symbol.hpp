/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SYMBOL_HPP)
#define BOOST_SPIRIT_PRANA_SYMBOL_HPP

#include <climits>

#include <boost/ref.hpp>
#include <boost/swap.hpp>
#include <boost/integer.hpp>
#include <boost/detail/iterator.hpp>

#include <boost/type_traits/alignment_of.hpp>

#include <boost/spirit/home/prana/kind.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Char>
class symbol: public boost::detail::iterator_traits<Char const*> {
 public:
  typedef typename boost::detail::iterator_traits<Char const*> iterator_traits;
 
  typedef typename iterator_traits::reference const const_reference;
  typedef typename iterator_traits::pointer const   const_pointer;
  typedef boost::uint_t<sizeof(void*)>::exact       size_type;
  typedef Char const*                               iterator;
  typedef Char const*                               const_iterator;
  typedef boost::reference_wrapper<Char const*>     ref_iterator;

  BOOST_STATIC_CONSTANT(size_type, stack_size =
    boost::alignment_of<void*[2]>::value / sizeof(Char));

  BOOST_STATIC_CONSTANT(size_type, meta_size =
    boost::alignment_of<void*>::value / sizeof(Char));

  struct stack_store {
    Char      str[stack_size];
    char      meta[meta_size];      
  };
    
  struct heap_store {
    Char*     str;
    size_type size;
  };

 private:
  union {
    stack_store stack;
    heap_store  heap;
  };

 public:
  void default_construct (void);
 
  void copy (symbol const&);
  template<typename Iterator> void copy (Iterator, Iterator);
 
  static symbol make (void);
  template<typename Iterator> static symbol make (Iterator, Iterator);
  
  void swap (symbol& other);

  void free (void);
  
  template<typename Container> Container get (void) const;

  iterator begin (void) const;
  iterator end (void) const;

  // alias for begin
  iterator str (void) const;
  
  char type (void) const;  
  void type (char);

  size_type size (void) const;
};

#define STORAGE     stack.meta[0]
#define TYPE        stack.meta[1]
#define HEAP        ((1 << CHAR_BIT) - 1)
#define HEAP_SIZE   heap.size
#define STACK_SIZE  stack.meta[0]

template<typename Char>
inline void symbol<Char>::default_construct (void) {
  heap.str = 0;
  heap.size = 0;

  // FIXME: there's gotta be some way to unroll this with TMP
  for (size_type i = 0; i != meta_size; ++i) stack.meta[i] = 0;
}

template<typename Char>
inline void symbol<Char>::copy (symbol const& other) {
  copy(other.str(), other.str() + other.size());
}

template<typename Char>
template<typename Iterator>
inline void symbol<Char>::copy (Iterator f, Iterator l) {
  free();

  size_type size = l - f;
  Char* str = 0;

  if (size <= stack_size) {
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
inline typename symbol<Char>::iterator symbol<Char>::str (void) const {
  return begin();
}

template<typename Char>
inline void symbol<Char>::swap (symbol& other) {
  boost::swap(*this, other);
}

template<typename Char>
inline char symbol<Char>::type (void) const {
  return TYPE;
}

template<typename Char>
inline void symbol<Char>::type (char t) {
  TYPE = t;
}

template<typename Char>
inline typename symbol<Char>::size_type symbol<Char>::size (void) const {
  if (STORAGE & HEAP) return HEAP_SIZE;
  return STACK_SIZE;
}

#undef STORAGE     
#undef TYPE        
#undef HEAP       
#undef HEAP_SIZE   
#undef STACK_SIZE  

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SYMBOL_HPP
