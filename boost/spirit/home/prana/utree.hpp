/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_UTREE_HPP)
#define BOOST_SPIRIT_PRANA_UTREE_HPP

#include <boost/cstdint.hpp>

#include <boost/spirit/home/prana/adt/sequence.hpp>
#include <boost/spirit/home/prana/adt/symbol.hpp>
#include <boost/spirit/home/prana/adt/range.hpp>
#include <boost/spirit/home/prana/kinds.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/policy/default.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Policy = policy::default_>
class utree {
 public:
  typedef typename Policy::char_type     char_type;
  typedef typename Policy::error_handler error_handler;

  typedef utree          value_type;
  typedef utree&         reference;
  typedef utree const&   const_reference;
  typedef utree*         pointer;
  typedef utree const*   const_pointer;
  typedef std::ptrdiff_t difference_type;
  typedef std::size_t    size_type;
  
  typedef adt::sequence<utree>   sequence;
  typedef adt::symbol<char_type> symbol;

  utree (void);

  utree (utree const&);
  template<typename Copier> utree (utree const&, Copier);
  utree (bool);
  utree (char);
  utree (int);
  utree (double);
  utree (char const*);
  utree (char const*, char const*);
  utree (char const*, size_type);
  utree (std::string const&); 
  
  utree& operator= (utree const&); 
  utree& operator= (bool);
  utree& operator= (char);
  utree& operator= (int);
  utree& operator= (double);
  utree& operator= (char const*);
  utree& operator= (std::string const&); 
  
  utree& copy (utree const&);
  template<typename Copier> utree& copy (utree const&, Copier);
  utree& copy (bool);
  utree& copy (char);
  utree& copy (int);
  utree& copy (double);
  utree& copy (char const*);
  utree& copy (char const*, char const*);
  utree& copy (char const*, size_type);
  utree& copy (std::string const&); 

  boost::uint8_t kind (void) const;

 private:
  boost::uint8_t _kind;
  union {
    pointer           _pointer; 
    sequence          _sequence;
    symbol            _symbol;
    bool              _bool;
    boost::intmax_t   _integer;
    double            _floating;
  }; 
};

template<typename Policy>
utree<Policy>::utree (void) {
  _kind = nil_kind; 
}

template<typename Policy>
utree<Policy>::utree (utree const& other) {
  copy(other);
}

template<typename Policy>
template<typename Copier>
utree<Policy>::utree (utree const& other, Copier copier) {
  copy(other, copier);
}

template<typename Policy>
utree<Policy>::utree (bool bool_) {
  copy(bool_);
}

template<typename Policy>
utree<Policy>::utree (char char_) {
  copy(char_);
}

template<typename Policy>
utree<Policy>::utree (int int_) {
  copy(int_);
}

template<typename Policy>
utree<Policy>::utree (double double_) {
  copy(double_);
}

template<typename Policy>
utree<Policy>::utree (char const* str) {
  copy(str);
}

template<typename Policy>
utree<Policy>::utree (char const* first, char const* last) {
  copy(first, last);
}

template<typename Policy>
utree<Policy>::utree (char const* bits, size_type len) {
  copy(bits, len);
}

template<typename Policy>
inline utree<Policy>::utree (std::string const& str) {
  copy(str);
}

template<typename Policy>
inline utree<Policy>& utree<Policy>::operator= (utree const& other) {
  copy(other);
}

template<typename Policy>
inline utree<Policy>& utree<Policy>::operator= (bool bool_) {
  copy(bool_);
}

template<typename Policy>
inline utree<Policy>& utree<Policy>::operator= (char char_) {
  copy(char_);
}

template<typename Policy>
inline utree<Policy>& utree<Policy>::operator= (int int_) {
  copy(int_);
}

template<typename Policy>
inline utree<Policy>& utree<Policy>::operator= (double double_) {
  copy(double_);
}

template<typename Policy>
inline utree<Policy>& utree<Policy>::operator= (char const* str) {
  copy(str);
}

template<typename Policy>
inline utree<Policy>& utree<Policy>::operator= (std::string const& str) {
  copy(str);
}

template<typename Policy>
inline utree<Policy>& utree<Policy>::copy (utree const& other) {
  copy(other, typename Policy::copier());
}

template<typename Policy>
template<typename Copier>
utree<Policy>& utree<Policy>::copy (utree const& other, Copier copier) {
  copier(other, other._kind, *this, _kind);
}

template<typename Policy>
utree<Policy>& utree<Policy>::copy (bool bool_) {
  _bool = bool_;
  _kind = bool_kind; 
}

template<typename Policy>
utree<Policy>& utree<Policy>::copy (char char_) {
  _symbol.default_construct();
  _symbol.deep_copy((char const*) &char_, (char const*) 0); 
  _kind = symbol_kind; 
}

template<typename Policy>
utree<Policy>& utree<Policy>::copy (int int_) {
  _integer = int_;
  _kind = integer_kind;
}

template<typename Policy>
utree<Policy>& utree<Policy>::copy (double double_) {
  _floating = double_;
  _kind = floating_kind; 
}

template<typename Policy>
utree<Policy>& utree<Policy>::copy (char const* str) {
  _symbol.default_construct();
  _symbol.deep_copy(str);
  _kind = symbol_kind; 
}

template<typename Policy>
utree<Policy>& utree<Policy>::copy (char const* first, char const* last) {
  _symbol.default_construct();
  _symbol.deep_copy(first, last);
  _kind = symbol_kind; 
}

template<typename Policy>
utree<Policy>& utree<Policy>::copy (char const* bits, size_type len) {
  _symbol.default_construct();
  _symbol.deep_copy(bits, bits + len);
  _kind = symbol_kind; 
}

template<typename Policy>
utree<Policy>& utree<Policy>::copy (std::string const& str) {
  _symbol.default_construct();
  _symbol.deep_copy(str);
  _kind = symbol_kind; 
}

template<typename Policy>
boost::uint8_t utree<Policy>::kind (void) const {
  return _kind | ~reference_kind;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_UTREE_HPP
