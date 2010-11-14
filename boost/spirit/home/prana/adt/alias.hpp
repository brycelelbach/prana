/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_ALIAS_HPP)
#define BOOST_SPIRIT_PRANA_ADT_ALIAS_HPP

#include <cstddef>
#include <cstring>

#include <boost/cstdint.hpp>

#include <boost/spirit/home/prana/kind.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Data>
struct alias {
  typedef Data        value_type;
  typedef Data&       reference;
  typedef Data const& const_reference;
  typedef Data*       pointer;
  typedef Data const* const_pointer;
  typedef std::size_t size_type;
  
  typedef boost::uint8_t metadata;
  
  struct storage {
    metadata _control [(sizeof(void*[1]) / 2)];
    pointer  _pointer;
  }; 

  void default_construct (void);

  void copy (alias const&);
  template<typename Pointer>
    void copy (Pointer);

  void clear (void);

        reference operator* (void);
  const_reference operator* (void) const;

        pointer operator-> (void);
  const_pointer operator-> (void) const;

        pointer get (void);
  const_pointer get (void) const;


  operator bool (void) const;

  bool operator== (alias const&) const;
  bool operator!= (alias const&) const;
  
  metadata kind (void) const;
  void kind (metadata);

  storage _data;
};

template<typename Data>
inline void alias<Data>::default_construct (void) {
  clear();
  _data._control[0] = reference_kind;
}

template<typename Data>
inline void alias<Data>::copy (alias const& other_) {
  if (*this != other_) 
    _data = other_._data;
}

template<typename Data>
template<typename Pointer>
inline void alias<Data>::copy (Pointer ptr_) {
  if (_data._pointer != ptr_)
    _data._pointer = ptr_;
}

template<typename Data>
inline void alias<Data>::clear (void) {
  std::memset(&_data, 0, sizeof(storage));
}

template<typename Data>
inline typename alias<Data>::reference alias<Data>::operator* (void) {
  return *_data._pointer;
}

template<typename Data>
inline typename alias<Data>::const_reference
alias<Data>::operator* (void) const {
  return *_data._pointer;
}

template<typename Data>
inline typename alias<Data>::pointer alias<Data>::operator-> (void) {
  return _data._pointer;
}

template<typename Data>
inline typename alias<Data>::const_pointer
alias<Data>::operator-> (void) const {
  return _data._pointer;
}

template<typename Data>
inline typename alias<Data>::pointer alias<Data>::get (void) {
  return _data._pointer;
}

template<typename Data>
inline typename alias<Data>::const_pointer alias<Data>::get (void) const {
  return _data._pointer;
}

template<typename Data>
inline alias<Data>::operator bool (void) const {
  return (bool) _data._pointer;
}

template<typename Data>
inline bool alias<Data>::operator== (alias const& other_) const {
  return _data._pointer == other_._data._pointer;
}

template<typename Data>
inline bool alias<Data>::operator!= (alias const& other_) const {
  return !operator==(other_); 
}

template<typename Data>
inline typename alias<Data>::metadata alias<Data>::kind (void) const {
  return _data._control[0];
}

template<typename Data>
inline void alias<Data>::kind (metadata kind_) {
  _data._control[0] = kind_;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_ALIAS_HPP
