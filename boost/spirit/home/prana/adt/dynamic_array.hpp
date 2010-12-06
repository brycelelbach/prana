/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_DYNAMIC_ARRAY_HPP)
#define BOOST_SPIRIT_PRANA_ADT_DYNAMIC_ARRAY_HPP

#include <cstring>

#include <iterator>

#include <boost/config.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/modulus.hpp>
#include <boost/mpl/not.hpp>

#include <boost/type_traits/is_pod.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <boost/spirit/home/support/container.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/support/half_t.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[dynamic_array_declaration
template<class Data, uinthalf_t InitialCapacity = 8>
class dynamic_array;
//]

//[dynamic_array_definition
template<class Data, uinthalf_t InitialCapacity>
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

  dynamic_array (void);
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
    void push (Value const&);
  
  template<class Value>
    void pop (void);

        iterator begin (void);
  const_iterator begin (void) const;

        iterator end (void);
  const_iterator end (void) const;

        reference front (void);
  const_reference front (void) const;
        
        reference back (void);
  const_reference back (void) const;

  template<class Container>
    Container get (void) const;

 private:
  template<class Iterator>
    void copy (Iterator, Iterator);

  size_type _size;
  size_type _capacity;
  Data* _data;
};
//]

template<class Data, uinthalf_t InitialCapacity>
dynamic_array<Data, InitialCapacity>::dynamic_array (void):
  _size(0), _capacity(InitialCapacity), _data(new Data[InitialCapacity]) { }

template<class Data, uinthalf_t InitialCapacity>
dynamic_array<Data, InitialCapacity>::dynamic_array (dynamic_array const& da) {
  copy(da.begin(), da.end());
}

template<class Data, uinthalf_t InitialCapacity>
template<class Container>
dynamic_array<Data, InitialCapacity>::dynamic_array (Container const& c) {
  copy(c.begin(), c.end());
}

template<class Data, uinthalf_t InitialCapacity>
template<class Iterator>
dynamic_array<Data, InitialCapacity>::dynamic_array (Iterator first,
                                                     Iterator last) {
  copy(first, last);
}

template<class Data, uinthalf_t InitialCapacity>
dynamic_array<Data, InitialCapacity>::~dynamic_array (void) {
  delete[] _data;
}

template<class Data, uinthalf_t InitialCapacity>
dynamic_array<Data, InitialCapacity>&
dynamic_array<Data, InitialCapacity>::operator= (dynamic_array const& da) {
  if (*this != da)
    copy(da.begin(), da.end());
  return *this;
}

template<class Data, uinthalf_t InitialCapacity>
template<class Container>
dynamic_array<Data, InitialCapacity>&
dynamic_array<Data, InitialCapacity>::operator= (Container const& c) {
  if (*this != c)
    copy(c.begin(), c.end());
  return *this;
}

template<class Data, uinthalf_t InitialCapacity>
void dynamic_array<Data, InitialCapacity>::assign (dynamic_array const& da) {
  if (*this != da)
    copy(da.begin(), da.end());
  return *this;

}

template<class Data, uinthalf_t InitialCapacity>
template<class Container>
void dynamic_array<Data, InitialCapacity>::assign (Container const& c) {
  if (*this != c)
    copy(c.begin(), c.end());
  return *this;
}

template<class Data, uinthalf_t InitialCapacity>
template<class Iterator>
void dynamic_array<Data, InitialCapacity>::assign (Iterator first,
                                                   Iterator last) {
  if (std::equal(first, last, _data))
    copy(first, last);
  return *this;
}

template<class Data, uinthalf_t InitialCapacity>
typename dynamic_array<Data, InitialCapacity>::size_type
dynamic_array<Data, InitialCapacity>::size (void) const {
  return _size;
}

template<class Data, uinthalf_t InitialCapacity>
typename dynamic_array<Data, InitialCapacity>::size_type
dynamic_array<Data, InitialCapacity>::capacity (void) const {
  return _capacity;
}

template<class Data, uinthalf_t InitialCapacity>
bool dynamic_array<Data, InitialCapacity>::empty (void) const {
  return _size;
}

template<class Data, uinthalf_t InitialCapacity>
void dynamic_array<Data, InitialCapacity>::clear (void) {
  std::memset(_data, 0, _size);
}
//[dynamic_array_insertion_algorithm
template<class Data, uinthalf_t InitialCapacity>
template<class Value>
void dynamic_array<Data, InitialCapacity>::push (Value const& val) {
  if (_size == _capacity) {
    _capacity *= 2; /*< Resize to twice its current capacity. >*/

    Data* new_data = new Data[_capacity]; /*< Allocate the new block. >*/

    std::memcpy(_data, new_data, _size); /*< Copy the old data to the newly
                                             allocated block. >*/
    delete[] _data;
  }

  _data[_size] = val;
  _size += 1;
} 

template<class Data, uinthalf_t InitialCapacity>
template<class Value>
void dynamic_array<Data, InitialCapacity>::pop (void) {
  _size -= 1;
}

template<class Data, uinthalf_t InitialCapacity>
typename dynamic_array<Data, InitialCapacity>::iterator
dynamic_array<Data, InitialCapacity>::begin (void) {
  return &_data[0];
}

template<class Data, uinthalf_t InitialCapacity>
typename dynamic_array<Data, InitialCapacity>::const_iterator
dynamic_array<Data, InitialCapacity>::begin (void) const {
  return &_data[0];
}

template<class Data, uinthalf_t InitialCapacity>
typename dynamic_array<Data, InitialCapacity>::iterator
dynamic_array<Data, InitialCapacity>::end (void) {
  return &_data[_size];
}

template<class Data, uinthalf_t InitialCapacity>
typename dynamic_array<Data, InitialCapacity>::const_iterator
dynamic_array<Data, InitialCapacity>::end (void) const {
  return &_data[_size];
}

template<class Data, uinthalf_t InitialCapacity>
typename dynamic_array<Data, InitialCapacity>::reference
dynamic_array<Data, InitialCapacity>::front (void) {
  return _data[0];
}

template<class Data, uinthalf_t InitialCapacity>
typename dynamic_array<Data, InitialCapacity>::const_reference
dynamic_array<Data, InitialCapacity>::front (void) const {
  return _data[0];
}

template<class Data, uinthalf_t InitialCapacity>
typename dynamic_array<Data, InitialCapacity>::reference
dynamic_array<Data, InitialCapacity>::back (void) {
  return _data[_size - 1];
}

template<class Data, uinthalf_t InitialCapacity>
typename dynamic_array<Data, InitialCapacity>::const_reference
dynamic_array<Data, InitialCapacity>::back (void) const {
  return _data[_size - 1];
}

template<class Data, uinthalf_t InitialCapacity>
template<class Container>
Container dynamic_array<Data, InitialCapacity>::get (void) const {
  return Container(begin(), end());
}
 
template<class Data, uinthalf_t InitialCapacity>
template<class Iterator>
void dynamic_array<Data, InitialCapacity>::copy (Iterator first,
                                                 Iterator last) {
  for (_size = 0; first != last; ++first, ++_size) {
    if (_size == _capacity) {
      _capacity *= 2; 
      Data* new_data = new Data[_capacity]; 
      std::memcpy(_data, new_data, _size); 
      delete[] _data;
    }

    _data[_size] = *first;
  }
}

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_ADT_DYNAMIC_ARRAY_HPP ->*/
