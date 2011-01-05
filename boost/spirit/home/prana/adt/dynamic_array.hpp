/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_DYNAMIC_ARRAY_HPP)
#define BOOST_SPIRIT_PRANA_ADT_DYNAMIC_ARRAY_HPP

#include <memory>
#include <algorithm>
#include <cstring>

#include <boost/config.hpp>
#include <boost/detail/iterator.hpp>

#include <boost/spirit/home/support/container.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/support/half_t.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Data, uinthalf_t Initial = 8,
         class Alloc = std::allocator<Data> >
class dynamic_array {
 public:
  typedef Data value_type;
  typedef Data& reference;
  typedef Data const& const_reference;
  typedef Data* pointer;
  typedef Data const* const_pointer;
  typedef ptrdiff_t difference_type;
  typedef uinthalf_t size_type;

  typedef Data* iterator;
  typedef Data const* const_iterator;

  dynamic_array (size_type init = Initial);
  dynamic_array (dynamic_array const&);
  template<class Container>
    dynamic_array (Container const&);
  template<class Iterator>
    dynamic_array (Iterator, Iterator);

  ~dynamic_array (void);

  dynamic_array& operator= (dynamic_array const&);
  template<class Container>
    dynamic_array& operator= (Container const&);
  
  void assign (dynamic_array const&);
  template<class Container>
    void assign (Container const&);
  template<class Iterator>
    void assign (Iterator, Iterator);

  size_type size (void) const;

  size_type capacity (void) const;

  bool empty (void) const;

  void clear (void);

  template<class Value>
    void push_back (Value const&);
  
  void pop_back (void);

        iterator begin (void);
  const_iterator begin (void) const;

        iterator end (void);
  const_iterator end (void) const;

        reference front (void);
  const_reference front (void) const;
        
        reference back (void);
  const_reference back (void) const;
        
        reference operator[] (size_type);
  const_reference operator[] (size_type) const;

  void reserve (size_type);
    
  template<class Container>
    Container get (void) const;

  template<class Container>
    bool operator== (Container const&) const;
  template<class Container>
    bool operator!= (Container const&) const;

 private:
  template<class Iterator>
    void copy (Iterator, Iterator);

  size_type _size;
  size_type _capacity;
  Alloc _alloc;
  Data* _data;
};

template<class Data, uinthalf_t Initial, class Alloc>
dynamic_array<Data, Initial, Alloc>::dynamic_array (size_type init):
  _size(0), _capacity(init), _alloc(),
  _data(_alloc.allocate(init)) { }

template<class Data, uinthalf_t Initial, class Alloc>
dynamic_array<Data, Initial, Alloc>::dynamic_array (dynamic_array const& da):
  _size(0), _capacity(0), _alloc(), _data(0)
{
  copy(da.begin(), da.end());
}

template<class Data, uinthalf_t Initial, class Alloc>
template<class Container>
dynamic_array<Data, Initial, Alloc>::dynamic_array (Container const& c):
  _size(0), _capacity(0), _alloc(), _data(0)
{
  copy(c.begin(), c.end());
}

template<class Data, uinthalf_t Initial, class Alloc>
template<class Iterator>
dynamic_array<Data, Initial, Alloc>::dynamic_array (Iterator first,
                                                    Iterator last):
  _size(0), _capacity(0), _alloc(), _data(0)
{
  copy(first, last);
}

template<class Data, uinthalf_t Initial, class Alloc>
dynamic_array<Data, Initial, Alloc>::~dynamic_array (void) {
  clear();
  if (_data && (_capacity != 0))
    _alloc.deallocate(_data, _capacity);
}

template<class Data, uinthalf_t Initial, class Alloc>
dynamic_array<Data, Initial, Alloc>&
dynamic_array<Data, Initial, Alloc>::operator= (dynamic_array const& da) {
  if (*this != da)
    copy(da.begin(), da.end());
  return *this;
}

template<class Data, uinthalf_t Initial, class Alloc>
template<class Container>
dynamic_array<Data, Initial, Alloc>&
dynamic_array<Data, Initial, Alloc>::operator= (Container const& c) {
  if (*this != c)
    copy(c.begin(), c.end());
  return *this;
}

template<class Data, uinthalf_t Initial, class Alloc>
void dynamic_array<Data, Initial, Alloc>::assign (dynamic_array const& da) {
  if (*this != da)
    copy(da.begin(), da.end());
  return *this;
}

template<class Data, uinthalf_t Initial, class Alloc>
template<class Container>
void dynamic_array<Data, Initial, Alloc>::assign (Container const& c) {
  if (*this != c)
    copy(c.begin(), c.end());
  return *this;
}

template<class Data, uinthalf_t Initial, class Alloc>
template<class Iterator>
inline void
dynamic_array<Data, Initial, Alloc>::assign (Iterator first, Iterator last) {
  if (std::equal(first, last, _data))
    copy(first, last);
  return *this;
}

template<class Data, uinthalf_t Initial, class Alloc>
typename dynamic_array<Data, Initial, Alloc>::size_type
dynamic_array<Data, Initial, Alloc>::size (void) const {
  return _size;
}

template<class Data, uinthalf_t Initial, class Alloc>
typename dynamic_array<Data, Initial, Alloc>::size_type
dynamic_array<Data, Initial, Alloc>::capacity (void) const {
  return _capacity;
}

template<class Data, uinthalf_t Initial, class Alloc>
bool dynamic_array<Data, Initial, Alloc>::empty (void) const {
  return _size;
}

template<class Data, uinthalf_t Initial, class Alloc>
void dynamic_array<Data, Initial, Alloc>::clear (void) {
  if (_data && (_size != 0)) {
    for (uinthalf_t i = 0; i < _size; ++i)
      _alloc.destroy(&_data[i]);
    _size = 0;  
  }
}

template<class Data, uinthalf_t Initial, class Alloc>
template<class Value>
void dynamic_array<Data, Initial, Alloc>::push_back (Value const& val) {
  if (_size == _capacity) {
    // allocate the new block
    Data* new_data = _alloc.allocate((_capacity ? _capacity * 2 : 1)); 
    std::memcpy(new_data, _data, _size * sizeof(Data)); // copy the old data

    // destroy and deallocate old data  
    for (uinthalf_t i = 0; i < _size; ++i)
      _alloc.destroy(&_data[i]);
    if (_capacity)
      _alloc.deallocate(_data, _capacity);

    _data = new_data;

    // resize to twice our current capacity
    if (_capacity)
      _capacity *= 2;
    else
      _capacity = 1;
  }

  _alloc.construct(&_data[_size], val);
  _size += 1;
} 

template<class Data, uinthalf_t Initial, class Alloc>
void dynamic_array<Data, Initial, Alloc>::pop_back (void) {
  _alloc.destroy(&_data[_size - 1]);
  _size -= 1;
}

template<class Data, uinthalf_t Initial, class Alloc>
typename dynamic_array<Data, Initial, Alloc>::iterator
dynamic_array<Data, Initial, Alloc>::begin (void) {
  return &_data[0];
}

template<class Data, uinthalf_t Initial, class Alloc>
typename dynamic_array<Data, Initial, Alloc>::const_iterator
dynamic_array<Data, Initial, Alloc>::begin (void) const {
  return &_data[0];
}

template<class Data, uinthalf_t Initial, class Alloc>
typename dynamic_array<Data, Initial, Alloc>::iterator
dynamic_array<Data, Initial, Alloc>::end (void) {
  return &_data[_size];
}

template<class Data, uinthalf_t Initial, class Alloc>
typename dynamic_array<Data, Initial, Alloc>::const_iterator
dynamic_array<Data, Initial, Alloc>::end (void) const {
  return &_data[_size];
}

template<class Data, uinthalf_t Initial, class Alloc>
typename dynamic_array<Data, Initial, Alloc>::reference
dynamic_array<Data, Initial, Alloc>::front (void) {
  return _data[0];
}

template<class Data, uinthalf_t Initial, class Alloc>
typename dynamic_array<Data, Initial, Alloc>::const_reference
dynamic_array<Data, Initial, Alloc>::front (void) const {
  return _data[0];
}

template<class Data, uinthalf_t Initial, class Alloc>
typename dynamic_array<Data, Initial, Alloc>::reference
dynamic_array<Data, Initial, Alloc>::back (void) {
  return _data[_size - 1];
}

template<class Data, uinthalf_t Initial, class Alloc>
typename dynamic_array<Data, Initial, Alloc>::const_reference
dynamic_array<Data, Initial, Alloc>::back (void) const {
  return _data[_size - 1];
}

template<class Data, uinthalf_t Initial, class Alloc>
typename dynamic_array<Data, Initial, Alloc>::reference
dynamic_array<Data, Initial, Alloc>::operator[] (size_type i) {
  return _data[i];
}

template<class Data, uinthalf_t Initial, class Alloc>
typename dynamic_array<Data, Initial, Alloc>::const_reference
dynamic_array<Data, Initial, Alloc>::operator[] (size_type i) const {
  return _data[i];
}

template<class Data, uinthalf_t Initial, class Alloc>
inline void
dynamic_array<Data, Initial, Alloc>::reserve (size_type s) {
  if (_capacity != s) {
    Data* new_data = _alloc.allocate(s); // allocate the new block

    if (_size) {
      if (_size > s)
        std::memcpy(new_data, _data, _size * sizeof(Data)); 
      else 
        std::memcpy(new_data, _data, s * sizeof(Data));
    }

    // destroy and deallocate old data  
    for (uinthalf_t i = 0; i < _size; ++i) {
      _alloc.destroy(&_data[i]);
    }

    if (_capacity)
      _alloc.deallocate(_data, _capacity);

    if (_size > s)
      _size = s;

    _data = new_data;
    _capacity = s; 
  } 
}

template<class Data, uinthalf_t Initial, class Alloc>
template<class Container>
Container dynamic_array<Data, Initial, Alloc>::get (void) const {
  return Container(begin(), end());
}
  
template<class Data, uinthalf_t Initial, class Alloc>
template<class Container>
inline bool
dynamic_array<Data, Initial, Alloc>::operator== (Container const& c) const {
  return (_size == c.size()) && std::equal(c.begin(), c.end(), &_data[0]);
}

template<class Data, uinthalf_t Initial, class Alloc>
template<class Container>
inline bool
dynamic_array<Data, Initial, Alloc>::operator!= (Container const& c) const {
  return !(*this == c);
}
 
template<class Data, uinthalf_t Initial, class Alloc>
template<class Iterator>
void dynamic_array<Data, Initial, Alloc>::copy (Iterator first, Iterator last) {
  typename boost::detail::iterator_traits<Iterator>::difference_type
    dist = boost::detail::distance(first, last);

  clear();

  if (first == last)
    return;

  // deallocate the old data and reallocate a big enough array
  if (dist > _capacity) {
    if (_data && (_capacity != 0))
      _alloc.deallocate(_data, _capacity);

    // figure out how large we need the array to be
    while (dist > _capacity) {
      if (_capacity)
        _capacity *= 2;
      else
        _capacity = 1;
    }

    // allocate it
    _data = _alloc.allocate(_capacity);
  }

  for (_size = 0; first != last; ++first, ++_size) {
    _alloc.construct(&_data[_size], *first);
  }
}

} // prana
} // spirit
} // boost 

#endif // BOOST_SPIRIT_PRANA_ADT_DYNAMIC_ARRAY_HPP

