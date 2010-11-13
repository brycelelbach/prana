/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_NUMERIC_HPP)
#define BOOST_SPIRIT_PRANA_ADT_NUMERIC_HPP

#include <cstddef>
#include <cstring>

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>

#include <boost/spirit/home/prana/kind.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct numeric {
  typedef boost::intmax_t         integer_type;
  typedef boost::intmax_t&        integer_reference;
  typedef boost::intmax_t const&  const_integer_reference;
  typedef double                  floating_type;
  typedef double&                 floating_reference;
  typedef double const&           const_floating_reference;

  typedef boost::uint8_t metadata;

  struct storage {
    metadata _control [(sizeof(void*[1]) / 2)];
    union {
      integer_type  _integer;
      floating_type _floating;
    };
  }; 

  void default_construct (void);
  
  void shallow_copy (numeric const&);
  template<typename Number>
    void shallow_copy (Number); 
 
  void deep_copy (numeric const&);
  template<typename Number>
    void deep_copy (Number); 
  
  void clear (void);

  bool operator== (numeric const&) const;
  bool operator!= (numeric const&) const;
  template<typename Number>
    bool operator== (Number) const;
  template<typename Number>
    bool operator!= (Number) const;

  template<typename Number>
    Number& get (void);
  template<typename Number>
    Number const& get (void) const;

  metadata kind (void) const;
  void kind (metadata);

  storage _data;
};

inline void numeric::default_construct (void) {
  clear();
}

template<typename Number> 
inline void numeric::shallow_copy (Number num_) {
  deep_copy(num_); 
}

inline void numeric::shallow_copy (numeric const& other_) {
  deep_copy(other_); 
}

template<>
inline void numeric::deep_copy<numeric::integer_type> (integer_type integer_) {
  clear();
  _data._control[0] = integer_kind;
  _data._integer = integer_;
} 

template<>
inline void numeric::deep_copy<numeric::floating_type> (floating_type double_) {
  clear();
  _data._control[0] = floating_kind;
  _data._floating = double_;
} 

template<>
inline void numeric::deep_copy<kind_type> (kind_type kind_) {
  clear();
  switch (kind_) {
    case nil_kind:
      _data._control[0] = nil_kind;
    case integer_kind:
      _data._control[0] = integer_kind;
    case floating_kind:
      _data._control[0] = floating_kind;
    default: BOOST_ASSERT("invalid kind for numeric");
  }
} 

inline void numeric::deep_copy (numeric const& other_) {
  if (*this != other_) 
    _data = other_._data;
}

inline void numeric::clear (void) {
  std::memset(&_data, 0, sizeof(storage));
}

inline bool numeric::operator== (numeric const& other_) const {
  switch (_data._control[0]) {
    case nil_kind:
      switch (other_._data._control[0]) {
        case nil_kind:
          // EXPLAIN (wash): nil trees are always 0, so this is always 0 == 0
          return true;
        case integer_kind:
          return other_._data._integer == 0; 
        case floating_kind:
          return other_._data._floating == 0;
        default: BOOST_ASSERT("invalid kind for numeric");
      }
    case integer_kind:
      switch (other_._data._control[0]) {
        case nil_kind:
          return 0 == _data._integer;
        case integer_kind:
          return other_._data._integer == _data._integer; 
        case floating_kind:
          return other_._data._floating == _data._integer;
        default: BOOST_ASSERT("invalid kind for numeric");
      }
    case floating_kind:
      switch (other_._data._control[0]) {
        case nil_kind:
          return 0 == _data._floating;
        case integer_kind:
          return other_._data._integer == _data._floating; 
        case floating_kind:
          return other_._data._floating == _data._floating;
        default: BOOST_ASSERT("invalid kind for numeric");
      }
    default: BOOST_ASSERT("invalid kind for numeric");
  }
  return false;
}

inline bool numeric::operator!= (numeric const& other_) const {
  return !operator==(other_);
}

template<typename Number>
inline bool numeric::operator== (Number num_) const {
  switch (_data._control[0]) {
    case nil_kind:
      return num_ == 0;
    case integer_kind:
      return num_ == _data._integer;
    case floating_kind:
      return num_ == _data._floating;
    default: BOOST_ASSERT("invalid kind for numeric");
  }
  return false;
}

template<typename Number>
inline bool numeric::operator!= (Number num_) const {
  return !operator==(num_);
}

template<>
inline numeric::integer_reference
numeric::get<numeric::integer_type> (void) {
  return _data._integer;
}

template<>
inline numeric::const_integer_reference
numeric::get<numeric::integer_type> (void) const {
  return _data._integer;
}

template<>
inline numeric::floating_reference
numeric::get<numeric::floating_type> (void) {
  return _data._floating;
}

template<>
inline numeric::const_floating_reference
numeric::get<numeric::floating_type> (void) const {
  return _data._floating;
}

inline numeric::metadata numeric::kind (void) const {
  return _data._control[0];
}

inline void numeric::kind (metadata kind_) {
  _data._control[0] = kind_;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_NUMERIC_HPP
