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

#include <boost/swap.hpp>
#include <boost/call_traits.hpp>

#include <boost/spirit/home/prana/adt/symbol.hpp>
#include <boost/spirit/home/prana/adt/sequence.hpp>
#include <boost/spirit/home/prana/adt/range.hpp>
#include <boost/spirit/home/prana/tag.hpp>
#include <boost/spirit/home/prana/kind.hpp>

namespace boost {
namespace spirit {
namespace prana {

class utree {
 public:
  typedef utree          value_type;
  typedef utree&         reference;
  typedef utree const&   const_reference;
  typedef utree*         pointer;
  typedef utree const*   const_pointer;
  typedef std::ptrdiff_t difference_type;
  typedef std::size_t    size_type;

  typedef sequence<utree>::iterator iterator;
  typedef sequence<utree>::const_iterator const_iterator;

  union discriminated_union {
    pointer           _pointer; 
    sequence<utree>   _sequence;
    symbol<char>      _symbol;
    bool              _bool;
    boost::intmax_t   _integer;
    double            _floating;
  };

  utree (void);
  utree (reference);
  template<class T>
    utree (typename call_traits<T>::param_type);
  template<class T0, class T1>
    utree (typename call_traits<T0>::param_type,
           typename call_traits<T1>::param_type);
 
  reference operator= (reference); 
  template<class T>
    reference operator= (typename call_traits<T>::param_type);
 
  void assign (reference); 
  template<class T>
    void assign (typename call_traits<T>::param_type);
  template<class T0, class T1>
    void assign (typename call_traits<T0>::param_type,
                 typename call_traits<T1>::param_type);

  void swap (reference);

  boost::uint8_t kind (void) const;

  void clear (void);
  
  template<kind_type Kind>
    void become (void);

  template<class Value>
    void push_front (typename call_traits<Value>::param_type);
  template<class Value>
    void push_back (typename call_traits<Value>::param_type);
  
  void pop_front (void);
  void pop_back (void);

  template<class Value>
    bool insert (typename call_traits<Value>::param_type, iterator);

  bool erase (iterator);

  iterator begin (void);
  const_iterator begin (void) const;

  iterator end (void);
  const_iterator end (void) const;

  bool operator== (const_reference) const;
  bool operator!= (const_reference) const;

 private:
  friend struct shallow_copy;
  friend struct deep_copy;

  template<typename Tree, typename Copy>
    void copy (Tree, Copy);
  void copy (const_reference);
  void copy (reference);
  void copy (bool);
  void copy (char);
  void copy (int);
  void copy (double);
  void copy (char const*);
  void copy (char const*, char const*);
  void copy (char const*, size_type);
  void copy (std::string const&);
 
  boost::uint8_t       _kind;
  discriminated_union  _du;
};

utree::utree (void) {
  std::memset(&_du, 0, sizeof(discriminated_union));
  _kind = nil_kind; 
}

utree::utree (reference other) {
  copy(other);
}

template<class T>
utree::utree (typename call_traits<T>::param_type t) {
  copy(t);
}

template<class T0, class T1>
utree::utree (typename call_traits<T0>::param_type t0,
              typename call_traits<T1>::param_type t1) {
  copy(t0, t1);
}

inline utree::reference utree::operator= (reference other) {
  copy(other);
}

template<class T>
inline utree::reference
utree::operator= (typename call_traits<T>::param_type t) {
  copy(t);
}

void utree::assign (reference other) {
  copy(other);
}

template<class T>
void utree::assign (typename call_traits<T>::param_type t) {
  copy(t);
}

template<class T0, class T1>
void utree::assign (typename call_traits<T0>::param_type t0,
                    typename call_traits<T1>::param_type t1) {
  copy(t0, t1);
}

inline void utree::swap (reference other) {
  boost::swap(_kind, other._kind);
  boost::swap(_du, other._du);
} 

inline boost::uint8_t utree::kind (void) const {
  return _kind;
}

void utree::clear (void) {
  if (!(_kind & reference_kind)) {
    switch ((kind_type) _kind) {
      case nil_kind:
      case bool_kind:
      case integer_kind:
      case floating_kind:
        break; 
      case symbol_kind:
        _du._symbol.clear();
        break; 
      case sequence_kind:
        _du._sequence.clear();
        break; 
      case record_kind: // TODO (wash): Requires the implementation of record.
        break; 
      default: BOOST_ASSERT("kind is only valid when masking another kind");
    }
  }

  _kind = nil_kind;
  std::memset(&_du, 0, sizeof(discriminated_union));
}

template<>
inline void utree::become<nil_kind> (void) {
  clear();
}

template<>
inline void utree::become<bool_kind> (void) {
  clear();
  _kind = bool_kind;
}

template<>
inline void utree::become<integer_kind> (void) {
  clear();
  _kind = integer_kind;
}

template<>
inline void utree::become<floating_kind> (void) {
  clear();
  _kind = floating_kind;
}

template<>
inline void utree::become<symbol_kind> (void) {
  clear();
  _kind = symbol_kind;
  _du._symbol.default_construct();
}

template<>
inline void utree::become<sequence_kind> (void) {
  clear();
  _kind = sequence_kind;
  _du._sequence.default_construct();
}

template<class Value>
void utree::push_front (typename call_traits<Value>::param_type val) {
  if (_kind & reference_kind) return _du._pointer->push_front(val);

  switch ((kind_type) _kind) {
    case nil_kind:
    case bool_kind:
    case integer_kind:
    case floating_kind:
    case record_kind:
    case symbol_kind: {
      utree old_(*this, deep);
      become<sequence_kind>();
      _du._sequence.push_front(old_);
      _du._sequence.push_front(val);
      return;
    }
    case sequence_kind:
      _du._sequence.push_front(val);
      return;
    default: BOOST_ASSERT("kind is only valid when masking another kind");
  } 
}

template<class Value>
void utree::push_back (typename call_traits<Value>::param_type val) {
  if (_kind & reference_kind) return _du._pointer->push_back(val);

  switch ((kind_type) _kind) {
    case nil_kind:
    case bool_kind:
    case integer_kind:
    case floating_kind:
    case record_kind:
    case symbol_kind: {
      utree old_(*this, deep);
      become<sequence_kind>();
      _du._sequence.push_back(old_);
      _du._sequence.push_back(val);
      return;
    }
    case sequence_kind:
      _du._sequence.push_back(val);
      return;
    default: BOOST_ASSERT("kind is only valid when masking another kind");
  } 
}

void utree::pop_front (void) {
  if (_kind & reference_kind) return _du._pointer->pop_front();
  
  switch ((kind_type) _kind) {
    case nil_kind:
    case bool_kind:
    case integer_kind:
    case floating_kind:
    case record_kind:
    case symbol_kind:
      become<nil_kind>();
      return;
    case sequence_kind:
      _du._sequence.pop_front();
      return;
    default: BOOST_ASSERT("kind is only valid when masking another kind");
  } 
}

void utree::pop_back (void) {
  if (_kind & reference_kind)
    return _du._pointer->pop_back();
  
  switch ((kind_type) _kind) {
    case nil_kind:
    case bool_kind:
    case integer_kind:
    case floating_kind:
    case record_kind:
    case symbol_kind:
      become<nil_kind>();
      return;
    case sequence_kind:
      _du._sequence.pop_back();
      return;
    default: BOOST_ASSERT("kind is only valid when masking another kind");
  } 
}

template<class Value>
inline bool
utree::insert (typename call_traits<Value>::param_type val, iterator pos) {
  if (_kind & reference_kind)
    return _du._pointer->insert(val, pos);

  else if (_kind == sequence_kind) {
    _du._sequence.insert(val, pos);
    return true;
  }

  else
    return false;
}

inline bool utree::erase (iterator pos) {
  if (_kind & reference_kind)
    return _du._pointer->erase(pos);

  else if (_kind == sequence_kind) {
    _du._sequence.erase(pos);
    return true;
  }

  else
    return false;
}

inline utree::iterator utree::begin (void) {
  if (_kind & reference_kind)
    return _du._pointer->begin();

  else if (_kind == sequence_kind)
    return _du._sequence.begin();

  else
    return iterator(0, 0);
}

inline utree::const_iterator utree::begin (void) const {
  if (_kind & reference_kind)
    return _du._pointer->begin();

  else if (_kind == sequence_kind)
    return _du._sequence.begin();

  else
    return const_iterator(0, 0);
}

inline utree::iterator utree::end (void) {
  if (_kind & reference_kind)
    return _du._pointer->end();

  else if (_kind == sequence_kind)
    return _du._sequence.end();

  else
    // EXPLAIN (wash): This means that --end() is invalid if the utree is not
    // a sequence or symbol.
    return iterator(0, 0);
}

inline utree::const_iterator utree::end (void) const {
  if (_kind & reference_kind)
    return _du._pointer->end();

  else if (_kind == sequence_kind)
    return _du._sequence.end();

  else
    return const_iterator(0, 0);
}

bool utree::operator== (const_reference rhs) const {
  if (rhs._kind & reference_kind)
    return *this == *rhs._du._pointer;

  else if (_kind & reference_kind)
    return *_du._pointer == rhs;

  else if (_kind != rhs._kind)
    return false;

  switch ((kind_type) _kind) {
    case nil_kind:
      return true; 
    case bool_kind:
      return _du._bool == rhs._du._bool;
    case integer_kind:
      return _du._integer == rhs._du._integer;
    case floating_kind:
      return _du._floating == rhs._du._floating;
    case symbol_kind:
      return _du._symbol == rhs._du._symbol;
    case sequence_kind:
      return _du._sequence == rhs._du._sequence;
    case record_kind:
      return false; // TODO (wash): Requires the implementation of record. 
    default: BOOST_ASSERT("kind is only valid when masking another kind");
  }
}

inline bool utree::operator!= (const_reference rhs) const {
  return !operator==(rhs); 
}

template<typename Tree, typename Copy>
inline void utree::copy (Tree other, Copy c) {
  clear();
  c(*this, _kind, other, other._kind);
}

inline void utree::copy (const_reference other) {
  clear();
  deep(*this, _kind, other, other._kind);
}

inline void utree::copy (reference other) {
  clear();
  shallow(*this, _kind, other, other._kind);
}

inline void utree::copy (bool bool_) {
  clear();
  _du._bool = bool_;
  _kind = bool_kind; 
}

inline void utree::copy (char char_) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(char_); 
  _kind = symbol_kind; 
}

inline void utree::copy (int int_) {
  clear();
  _du._integer = int_;
  _kind = integer_kind;
}

inline void utree::copy (double double_) {
  clear();
  _du._floating = double_;
  _kind = floating_kind; 
}

inline void utree::copy (char const* str) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(str);
  _kind = symbol_kind; 
}

inline void utree::copy (char const* first, char const* last) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(first, last);
  _kind = symbol_kind; 
}

inline void utree::copy (char const* bits, size_type len) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(bits, bits + len);
  _kind = symbol_kind;
}

inline void utree::copy (std::string const& str) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(str);
  _kind = symbol_kind;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_UTREE_HPP
