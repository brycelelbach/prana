/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_UTREE_HPP)
#define BOOST_SPIRIT_PRANA_UTREE_HPP

#include <cstring>

#include <boost/config.hpp>
#include <boost/swap.hpp>
#include <boost/cstdint.hpp>

#include <boost/spirit/home/prana/adt/sequence.hpp>
#include <boost/spirit/home/prana/adt/symbol.hpp>
#include <boost/spirit/home/prana/adt/range.hpp>
#include <boost/spirit/home/prana/kind.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/tag.hpp>
#include <boost/spirit/home/prana/policy/default.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<template<class> class Policy = policy::default_>
class utree {
 public:
  typedef Policy<utree> policy;

  typedef typename policy::char_type char_type;

  typedef utree          value_type;
  typedef utree&         reference;
  typedef utree const&   const_reference;
  typedef utree*         pointer;
  typedef utree const*   const_pointer;
  typedef std::ptrdiff_t difference_type;
  typedef std::size_t    size_type;
  
  typedef typename policy::container default_container;

  typedef adt::sequence<utree>   sequence;
  typedef adt::symbol<char_type> symbol;
  
  union discriminated_union {
    pointer           _pointer; 
    sequence          _sequence;
    symbol            _symbol;
    bool              _bool;
    boost::intmax_t   _integer;
    double            _floating;
  };

  utree (void);

  utree (const_reference);
  template<typename Copy> utree (const_reference, Copy);
  utree (bool);
  utree (char_type);
  utree (int);
  utree (double);
  utree (char_type const*);
  utree (char_type const*, char_type const*);
  utree (char_type const*, size_type);
  utree (std::basic_string<char_type> const&); 

  // TODO (wash): add equality checks to avoid unnecessary copies  
  utree& operator= (const_reference); 
  utree& operator= (bool);
  utree& operator= (char_type);
  utree& operator= (int);
  utree& operator= (double);
  utree& operator= (char_type const*);
  utree& operator= (std::basic_string<char_type> const&); 
  
  // TODO (wash): add assign convienence method 

  // TODO (wash): add equality checks to avoid unnecessary copies  
  void swap (reference);

  boost::uint8_t kind (void) const;

  void clear (void);

  discriminated_union& raw (void);
  discriminated_union const& raw (void) const; 

  default_container& container (void);
  default_container const& container (void) const;
  
  void become (kind_type);  

 private:
  utree& copy (utree const&);
  template<typename Copy> utree& copy (utree const&, Copy);
  utree& copy (bool);
  utree& copy (char_type);
  utree& copy (int);
  utree& copy (double);
  utree& copy (char_type const*);
  utree& copy (char_type const*, char_type const*);
  utree& copy (char_type const*, size_type);
  utree& copy (std::basic_string<char_type> const&);
 
  boost::uint8_t       _kind;
  discriminated_union  _du;
};

template<template<class> class Policy>
utree<Policy>::utree (void) {
  std::memset(&_du, 0, sizeof(discriminated_union));
  _kind = nil_kind; 
}

template<template<class> class Policy>
utree<Policy>::utree (const_reference other) {
  copy(other);
}

template<template<class> class Policy>
template<typename Copy>
utree<Policy>::utree (const_reference other, Copy copy) {
  copy(other, copy);
}

template<template<class> class Policy>
utree<Policy>::utree (bool bool_) {
  copy(bool_);
}

template<template<class> class Policy>
utree<Policy>::utree (char_type char_) {
  copy(char_);
}

template<template<class> class Policy>
utree<Policy>::utree (int int_) {
  copy(int_);
}

template<template<class> class Policy>
utree<Policy>::utree (double double_) {
  copy(double_);
}

template<template<class> class Policy>
utree<Policy>::utree (char_type const* str) {
  copy(str);
}

template<template<class> class Policy>
utree<Policy>::utree (char_type const* first, char_type const* last) {
  copy(first, last);
}

template<template<class> class Policy>
utree<Policy>::utree (char_type const* bits, size_type len) {
  copy(bits, len);
}

template<template<class> class Policy>
inline utree<Policy>::utree (std::basic_string<char_type> const& str) {
  copy(str);
}

template<template<class> class Policy>
inline utree<Policy>& utree<Policy>::operator= (const_reference other) {
  copy(other);
}

template<template<class> class Policy>
inline utree<Policy>& utree<Policy>::operator= (bool bool_) {
  copy(bool_);
}

template<template<class> class Policy>
inline utree<Policy>& utree<Policy>::operator= (char_type char_) {
  copy(char_);
}

template<template<class> class Policy>
inline utree<Policy>& utree<Policy>::operator= (int int_) {
  copy(int_);
}

template<template<class> class Policy>
inline utree<Policy>& utree<Policy>::operator= (double double_) {
  copy(double_);
}

template<template<class> class Policy>
inline utree<Policy>& utree<Policy>::operator= (char_type const* str) {
  copy(str);
}

template<template<class> class Policy>
inline utree<Policy>& 
utree<Policy>::operator= (std::basic_string<char_type> const& str) {
  copy(str);
}

template<template<class> class Policy>
inline void utree<Policy>::swap (reference other) {
  boost::swap(_kind, other._kind);
  boost::swap(_du, other._du);
} 

template<template<class> class Policy>
inline boost::uint8_t utree<Policy>::kind (void) const {
  return _kind; 
}

template<template<class> class Policy>
void utree<Policy>::clear (void) {
  if (!(_kind & reference_kind)) {
    switch ((kind_type) _kind) {
      case reference_kind:
      case numeric_kind:
      case container_kind:
        // DISCUSS (wash): The above three are impossible; they fall through to
        // nil_kind for now. Perhaps they should throw an error if hit? 
      case nil_kind:
        break; 
      case symbol_kind:
        _du._symbol.free();
        break; 
      case record_kind:
        break; // TODO (wash): Requires the implementation of record. 
      case bool_kind:
        break; 
      case integer_kind:
        break; 
      case floating_kind:
        break; 
      case sequence_kind:
        _du._sequence.free();
        break; 
      case array_kind:
        break; // TODO (wash): Requires the implementation of array.
      case unique_kind:
        break; // TODO (wash): Requires the implementation of unique.
    }
  }

  _kind = nil_kind;
  std::memset(&_du, 0, sizeof(discriminated_union));
}

template<template<class> class Policy>
inline typename utree<Policy>::discriminated_union&
utree<Policy>::raw (void) {
  return _du;
}

template<template<class> class Policy>
inline typename utree<Policy>::discriminated_union const&
utree<Policy>::raw (void) const {
  return _du;
}

template<template<class> class Policy>
inline typename utree<Policy>::default_container&
utree<Policy>::container (void) {
  return typename default_container::retrieve()(*this);
}

template<template<class> class Policy>
inline typename utree<Policy>::default_container const&
utree<Policy>::container (void) const {
  return typename default_container::retrieve()(*this);
}

template<template<class> class Policy>
void utree<Policy>::become (kind_type kind_) {
  switch ((kind_type) _kind) {
    case reference_kind:
    case numeric_kind:
    case container_kind:
      // DISCUSS (wash): The above three are impossible; they fall through to
      // nil_kind for now. Perhaps they should throw an error if hit? 
    case bool_kind:
    case integer_kind:
    case floating_kind:
    case nil_kind:
      clear();
      break; 
    case symbol_kind:
      clear();
      _du._symbol.default_construct();
      break; 
    case sequence_kind:
      clear();
      _du._sequence.default_construct();
      break; 
    case record_kind:
      clear();
      break; // TODO (wash): Requires the implementation of record. 
    case array_kind:
      clear();
      break; // TODO (wash): Requires the implementation of array.
    case unique_kind:
      clear();
      break; // TODO (wash): Requires the implementation of unique.
  }

  _kind = kind_;
}

template<template<class> class Policy>
inline utree<Policy>& utree<Policy>::copy (const_reference other) {
  copy(other, typename policy::copy());
}

template<template<class> class Policy>
template<typename Copy>
inline utree<Policy>& utree<Policy>::copy (
  const_reference other, Copy copy
) {
  clear();
  copy(other, other._kind, *this, _kind);
}

template<template<class> class Policy>
inline utree<Policy>& utree<Policy>::copy (bool bool_) {
  clear();
  _du._bool = bool_;
  _kind = bool_kind; 
}

template<template<class> class Policy>
inline utree<Policy>& utree<Policy>::copy (char_type char_) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy((char_type const*) &char_, (char_type const*) 0); 
  _kind = symbol_kind; 
}

template<template<class> class Policy>
inline utree<Policy>& utree<Policy>::copy (int int_) {
  clear();
  _du._integer = int_;
  _kind = integer_kind;
}

template<template<class> class Policy>
inline utree<Policy>& utree<Policy>::copy (double double_) {
  clear();
  _du._floating = double_;
  _kind = floating_kind; 
}

template<template<class> class Policy>
inline utree<Policy>& utree<Policy>::copy (char_type const* str) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(str);
  _kind = symbol_kind; 
}

template<template<class> class Policy>
inline utree<Policy>&
utree<Policy>::copy (char_type const* first, char_type const* last) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(first, last);
  _kind = symbol_kind; 
}

template<template<class> class Policy>
inline utree<Policy>&
utree<Policy>::copy (char_type const* bits, size_type len) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(bits, bits + len);
  _kind = symbol_kind;
}

template<template<class> class Policy>
inline utree<Policy>&
utree<Policy>::copy (std::basic_string<char_type> const& str) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(str);
  _kind = symbol_kind;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_UTREE_HPP
