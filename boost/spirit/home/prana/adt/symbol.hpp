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

#include <string>

#include <boost/call_traits.hpp>
#include <boost/cstdint.hpp>

#include <boost/spirit/home/prana/fn/length.hpp>
#include <boost/spirit/home/prana/tag.hpp>

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
  
  typedef boost::uint8_t metadata;
 
  // TODO (wash): We're a little bit wasteful (hehe, get it? -bit- wasteful)
  // here - alias could just be a bit in storage. Also, this can be restructured
  // into a structure with an anonymous union, to make member access less
  // painful.
  union storage {
    struct {
      metadata _control  [(sizeof(void*[1]) / 2)];
      metadata _storage;
      metadata _alias;
      Char     _str      [(sizeof(void*[2]) / sizeof(Char))];
    } _stack;
    struct {
      metadata _control  [(sizeof(void*[1]) / 2)];
      metadata _storage;
      metadata _alias;
      Char*    _first;
      Char*    _last;
    } _heap;
  };

  void default_construct (void);
  
  void shallow_copy (Char*);
  void shallow_copy (symbol&);
  template<typename Iterator>
    void shallow_copy (Iterator&, Iterator&);
  template<typename Container>
    void shallow_copy (Container&);
 
  void deep_copy (Char);
  void deep_copy (Char*);
  void deep_copy (Char const*);
  void deep_copy (symbol const&);
  template<typename Iterator>
    void deep_copy (Iterator, Iterator);
  template<typename Container>
    void deep_copy (typename call_traits<Container>::param_type);
  
  void clear (void);
  
  template<typename Container>
    Container get (void) const;

  iterator begin (void) const;
  iterator end (void) const;

  bool operator== (Char) const;
  bool operator== (Char*) const;
  bool operator== (Char const*) const;
  bool operator== (symbol const&) const;
  template<typename Container>
    bool operator== (Container) const;
  
  bool operator!= (Char) const;
  bool operator!= (Char*) const;
  bool operator!= (Char const*) const;
  bool operator!= (symbol const&) const;
  template<typename Container>
    bool operator!= (Container) const;
  
  std::basic_string<Char> str (void) const;

  size_type size (void) const;
  
  metadata kind (void) const;
  void kind (metadata);
 
  storage _data;
};

template<typename Char>
inline void symbol<Char>::default_construct (void) {
  std::memset(&_data, 0, sizeof(storage));
}

template<typename Char>
inline void symbol<Char>::shallow_copy (symbol& other_) {
  if (other_ != *this) { 
    std::memcpy(&_data, &other_._data, sizeof(storage));
    _data._stack._alias = true;
  }
}

template<typename Char>
inline void symbol<Char>::deep_copy (Char c_) {
  if (*this != &c_) { 
    clear();
    _data._stack._storage = 1;
    // EXPLAIN (wash): clear() zero initializes the discriminated union, so
    // there is no need to append a null byte.
    _data._stack._str[0] = c_;
  }
} 

template<typename Char>
inline void symbol<Char>::deep_copy (Char* c_) {
  deep_copy(c_, c_ + length()(c_));
}

template<typename Char>
inline void symbol<Char>::deep_copy (Char const* c_) {
  deep_copy(c_, c_ + length()(c_));
}

template<typename Char>
inline void symbol<Char>::deep_copy (symbol const& other_) {
  deep_copy(other_.begin(), other_.end());
}

template<typename Char>
template<typename Iterator>
inline void symbol<Char>::deep_copy (Iterator first_, Iterator last_) {
  if (std::equal(first_, last_, begin()) && !_data._stack._alias)
    return;

  clear();

  size_type const size = last_ - first_;
  static size_type const stack_size = (sizeof(void*[2]) / sizeof(Char));

  if (!size)
    return;

  else if (size <= stack_size) {
    _data._stack._storage = size;
  
    for (size_type i = 0; i != size; ++i)
      _data._stack._str[i] = *first_++;
  }

  else {
    _data._heap._storage = CHAR_MAX;
    _data._heap._first = new Char[size];

    for (size_type i = 0; i != size; ++i)
      _data._heap._first[i] = *first_++;
  
    _data._heap._last = &_data._heap._first[size];
  }
}

template<typename Char>
template<typename Container>
inline void symbol<Char>::deep_copy (
  typename call_traits<Container>::param_type c_
) {
  deep_copy(c_.begin(), c_.end());
}

template<typename Char>
inline void symbol<Char>::clear (void) { 
  if (_data._heap._storage == CHAR_MAX) 
    if (!_data._heap._alias)
      delete[] _data._heap._first;
  
  std::memset(&_data, 0, sizeof(storage));
}

template<typename Char>
template<typename Container>
inline Container symbol<Char>::get (void) const {
  if (!_data._stack._storage)
    return Container();
  return Container(begin(), end());
}

template<typename Char>
inline typename symbol<Char>::iterator symbol<Char>::begin (void) const {
  if (_data._heap._storage == CHAR_MAX)
    return _data._heap._first;
  return _data._stack._str;
}

template<typename Char>
inline typename symbol<Char>::iterator symbol<Char>::end (void) const {
  if (_data._heap._storage == CHAR_MAX)
    return _data._heap._last;
  return _data._stack._str + _data._stack._storage;
}

template<typename Char>
inline std::basic_string<Char> symbol<Char>::str (void) const {
  return get<std::basic_string<Char> >();
}

template<typename Char>
inline bool symbol<Char>::operator== (Char c_) const {
  const_iterator b = begin();
  return (b != end()) && (*b == c_);
}

template<typename Char>
inline bool symbol<Char>::operator== (Char* c_) const {
  return std::equal(begin(), end(), c_);
}

template<typename Char>
inline bool symbol<Char>::operator== (Char const* c_) const {
  return std::equal(begin(), end(), c_);
}

template<typename Char>
inline bool symbol<Char>::operator== (symbol const& other_) const {
  return !std::equal(other_.begin(), other_.end(), begin());
}

template<typename Char>
template<typename Container>
inline bool symbol<Char>::operator== (Container c_) const {
  return std::equal(c_.begin(), c_.end(), begin());
}

template<typename Char>
inline bool symbol<Char>::operator!= (Char c_) const {
  return !operator==(c_);
}

template<typename Char>
inline bool symbol<Char>::operator!= (Char* c_) const {
  return !operator==(c_);
}

template<typename Char>
inline bool symbol<Char>::operator!= (Char const* c_) const {
  return !operator==(c_);
}

template<typename Char>
inline bool symbol<Char>::operator!= (symbol const& other_) const {
  return !operator==(other_);
}

template<typename Char>
template<typename Container>
inline bool symbol<Char>::operator!= (Container c_) const {
  return !operator==(c_);
}

template<typename Char>
inline typename symbol<Char>::size_type symbol<Char>::size (void) const {
  if (_data._heap._storage == CHAR_MAX)
    return _data._heap._last - _data._heap._first;
  return _data._stack._storage;
}

template<typename Char>
inline typename symbol<Char>::metadata
symbol<Char>::kind (void) const {
  return _data._stack._control[0];
}

template<typename Char>
inline void symbol<Char>::kind (metadata kind_) {
  _data._stack._control[0] = kind_;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_SYMBOL_HPP
