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
#include <boost/spirit/home/prana/adt/numeric.hpp>
#include <boost/spirit/home/prana/adt/alias.hpp>
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
  typedef utree const*   const_alias;
  typedef std::ptrdiff_t difference_type;
  typedef std::size_t    size_type;

  typedef boost::uint8_t metadata;

  typedef sequence<utree>::iterator iterator;
  typedef sequence<utree>::const_iterator const_iterator;
 
  // TODO (wash): Putting this note here as I can't think of a better place for
  // it. In a few places, we set the kind right after a clear/default_construct
  // method call to a member of the discriminated union. This is done because
  // we used to store the kind in the utree class itself; now we store it in
  // the du members. We should therefore move the kind reset into the clear
  // methods. While we're at it, a clearly defined calling convention for the
  // clear/default_construct members is needed in general, so that we can make
  // some (safe) assumptions about clear/default_construct, and clean up the
  // codebase a bit so we don't rely on compiler optimizing (inlining, etc) in
  // as many places as we currently do. This is not a top priority, but it is
  // pretty important.
  union discriminated_union {
    alias<utree>    _alias; 
    sequence<utree> _sequence;
    symbol<char>    _symbol;
    numeric         _numeric;
  };

  utree (void);
  template<class T>
    utree (T);
  template<class T0, class T1>
    utree (T0, T1);
 
  template<class T>
    reference operator= (T);
 
  template<class T>
    void assign (T);
  template<class T0, class T1>
    void assign (T0, T1);

  template<class Tree>
    void swap (Tree&);

  metadata kind (void) const;

  void clear (void);
  void shallow_clear (void);
  void deep_clear (void);
  
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
  
        reference front (void);
  const_reference front (void) const;

        reference back (void);
  const_reference back (void) const;

  bool operator== (bool) const;
  bool operator== (char) const;
  bool operator== (boost::intmax_t) const;
  bool operator== (double) const;
  bool operator== (char*) const;
  bool operator== (char const*) const;
  bool operator== (std::string const&) const;
  template<class RHS>
    bool operator== (RHS) const;
  template<class RHS>
    bool operator!= (RHS) const;

 private:
  friend struct shallow_copy;
  friend struct deep_copy;

  template<typename Tree, typename Copy>
    void copy (Tree, Copy);
  void copy (const_reference);
  void copy (reference);
  template<typename Tree>
    void copy (Tree const&);
  template<typename Tree>
    void copy (Tree&);
  void copy (bool);
  void copy (char);
  void copy (boost::intmax_t);
  void copy (double);
  void copy (char*);
  void copy (char const*);
  void copy (char*, char*);
  void copy (char const*, char const*);
  void copy (char*, size_type);
  void copy (char const*, size_type);
  void copy (std::string const&);
 
  discriminated_union _du;
};

utree::utree (void) {
  std::memset(&_du, 0, sizeof(discriminated_union));
}

template<class T>
utree::utree (T t) {
  copy(t);
}

template<class T0, class T1>
utree::utree (T0 t0, T1 t1) {
  copy(t0, t1);
}

template<class T>
inline utree::reference utree::operator= (T t_) {
  if (*this != t_)
    copy(t_);
  return *this;
}

template<class T>
void utree::assign (T t_) {
  if (*this != t_)
    copy(t_);
}

template<>
void utree::assign<char*, char*> (char* first_, char* last_) {
  if (kind() & reference_kind)
    return _du._alias->assign(first_, last_);

  if (kind() != symbol_kind)
    copy(first_, last_);

  // DISCUSS (wash): Is this more expensive than copying an identical symbol?
  if (kind() == symbol_kind &&
      !std::equal(first_, last_, _du._symbol.begin()))
    copy(first_, last_); 
}

template<>
void utree::assign<char const*, char const*> (
  char const* first_, char const* last_
) {
  if (kind() & reference_kind)
    return _du._alias->assign(first_, last_);

  if (kind() != symbol_kind)
    copy(first_, last_);

  if (kind() == symbol_kind &&
      !std::equal(first_, last_, _du._symbol.begin()))
    copy(first_, last_); 
}

template<>
void utree::assign<char*, utree::size_type> (char* bits_, size_type len_) {
  if (kind() & reference_kind)
    return _du._alias->assign(bits_, len_);

  if (kind() != symbol_kind)
    copy(bits_, len_);

  if (kind() == symbol_kind &&
      !std::equal(bits_, bits_ + len_, _du._symbol.begin()))
    copy(bits_, len_); 
}

template<>
void utree::assign<char const*, utree::size_type> (
  char const* bits_, size_type len_
) {
  if (kind() & reference_kind)
    return _du._alias->assign(bits_, len_);

  if (kind() != symbol_kind)
    copy(bits_, len_);

  if (kind() == symbol_kind &&
      !std::equal(bits_, bits_ + len_, _du._symbol.begin()))
    copy(bits_, len_); 
}

template<class T0, class T1>
void utree::assign (T0 t0_, T1 t1_) {
  if (*this != t0_)
    copy(t0_, t1_);
}

template<class T>
inline void utree::swap (T& t_) {
  if (*this != t_)
    boost::swap(_du, t_._du);
} 

inline utree::metadata utree::kind (void) const {
  return _du._alias.kind();
}

void utree::clear (void) {
  deep_clear();
}

void utree::shallow_clear (void) {
  if (!(kind() & reference_kind)) {
    switch ((kind_type) kind()) {
      case nil_kind:
      case integer_kind:
      case floating_kind:
        _du._numeric.clear();
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
}

void utree::deep_clear (void) {
  if (kind() & reference_kind)  
    return _du._alias->shallow_clear();

  shallow_clear();
}

template<>
inline void utree::become<nil_kind> (void) {
  deep_clear();
}
template<>
inline void utree::become<integer_kind> (void) {
  deep_clear();
  _du._numeric.kind(integer_kind);
}

template<>
inline void utree::become<floating_kind> (void) {
  deep_clear();
  _du._numeric.kind(floating_kind);
}

template<>
inline void utree::become<symbol_kind> (void) {
  deep_clear();
  _du._symbol.kind(symbol_kind);
}

template<>
inline void utree::become<sequence_kind> (void) {
  deep_clear();
  _du._symbol.kind(sequence_kind);
}

template<class Value>
void utree::push_front (typename call_traits<Value>::param_type val) {
  if (kind() & reference_kind)
    return _du._alias->push_front(val);

  switch ((kind_type) kind()) {
    case nil_kind:
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
  if (kind() & reference_kind)
    return _du._alias->push_back(val);

  switch ((kind_type) kind()) {
    case nil_kind:
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
  if (kind() & reference_kind)
    return _du._alias->pop_front();
  
  switch ((kind_type) kind()) {
    case nil_kind:
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
  if (kind() & reference_kind)
    return _du._alias->pop_back();
  
  switch ((kind_type) kind()) {
    case nil_kind:
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
inline bool utree::insert (
  typename call_traits<Value>::param_type val, iterator pos
) {
  if (kind() & reference_kind)
    return _du._alias->insert(val, pos);

  else if (kind() == sequence_kind) {
    _du._sequence.insert(val, pos);
    return true;
  }

  else
    return false;
}

inline bool utree::erase (iterator pos) {
  if (kind() & reference_kind)
    return _du._alias->erase(pos);

  else if (kind() == sequence_kind) {
    _du._sequence.erase(pos);
    return true;
  }

  else
    return false;
}

inline utree::iterator utree::begin (void) {
  if (kind() & reference_kind)
    return _du._alias->begin();

  else if (kind() == sequence_kind)
    return _du._sequence.begin();

  // DISCUSS (wash): We need a better solution here; conversion to sequence is
  // not an option, though, because the const version of begin can't do such a
  // conversion, and different behavior for const begin and non-const begin
  // would be annoying.
  else
    return iterator(0, 0);
}

inline utree::const_iterator utree::begin (void) const {
  if (kind() & reference_kind)
    return _du._alias->begin();

  else if (kind() == sequence_kind)
    return _du._sequence.begin();

  else
    return const_iterator(0, 0);
}

inline utree::iterator utree::end (void) {
  if (kind() & reference_kind)
    return _du._alias->end();

  else if (kind() == sequence_kind)
    return _du._sequence.end();

  else
    return iterator(0, 0);
}

inline utree::const_iterator utree::end (void) const {
  if (kind() & reference_kind)
    return _du._alias->end();

  else if (kind() == sequence_kind)
    return _du._sequence.end();

  else
    return const_iterator(0, 0);
}

inline utree::reference utree::front (void) {
  if (kind() & reference_kind)
    return _du._alias->front();

  else if (kind() == sequence_kind)
    return *_du._sequence.begin();
  
  else
    return *this;
}

inline utree::const_reference utree::front (void) const {
  if (kind() & reference_kind)
    return _du._alias->front();

  else if (kind() == sequence_kind)
    return *_du._sequence.begin();
  
  else
    return *this;
}

inline utree::reference utree::back (void) {
  if (kind() & reference_kind)
    return _du._alias->back();

  else if (kind() == sequence_kind)
    return *--_du._sequence.end();

  else
    return *this;
}

inline utree::const_reference utree::back (void) const {
  if (kind() & reference_kind)
    return _du._alias->back();

  else if (kind() == sequence_kind)
    return *--_du._sequence.end();

  else
    return *this;
}

bool utree::operator== (bool bool_) const {
  if (kind() & reference_kind)
    return *_du._alias == bool_;

  // EXPLAIN (wash): We treat boolean comparison specially; unlike other numeric
  // equality comparisons, we compare against symbol/sequence size instead of
  // returning false; aka, utree == true is true if the utree is not an empty
  // string, empty container, or numeric 0.
  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
      return _du._numeric == bool_;
    case symbol_kind:
      return _du._symbol.size() == bool_;
    case sequence_kind:
      return _du._sequence.size() == bool_;
    case record_kind:
      return false; // TODO (wash): Requires the implementation of record. 
    default: BOOST_ASSERT("kind is only valid when masking another kind");
  }

  return false;
}

bool utree::operator== (char char_) const {
  if (kind() & reference_kind)
    return *_du._alias == char_;

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
      return false;
    case symbol_kind:
      return _du._symbol == char_;
    case sequence_kind:
      if (_du._sequence.size() == 1)
        return front() == char_;
      else
        return false;
    case record_kind:
      return false; // TODO (wash): Requires the implementation of record. 
    default: BOOST_ASSERT("kind is only valid when masking another kind");
  }

  return false;
}

bool utree::operator== (boost::intmax_t int_) const {
  if (kind() & reference_kind)
    return *_du._alias == int_;

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
      return _du._numeric == int_;
    case symbol_kind:
      return false;
    case sequence_kind:
      if (_du._sequence.size() == 1)
        return front() == int_;
      else
        return false;
    case record_kind:
      return false; // TODO (wash): Requires the implementation of record. 
    default: BOOST_ASSERT("kind is only valid when masking another kind");
  }

  return false;
}

bool utree::operator== (double double_) const {
  if (kind() & reference_kind)
    return *_du._alias == double_;

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
      return _du._numeric == double_;
    case symbol_kind:
      return false;
    case sequence_kind:
      if (_du._sequence.size() == 1)
        return front() == double_;
      else
        return false;
    case record_kind:
      return false; // TODO (wash): Requires the implementation of record. 
    default: BOOST_ASSERT("kind is only valid when masking another kind");
  }

  return false;
}

bool utree::operator== (char* str_) const {
  if (kind() & reference_kind)
    return *_du._alias == str_;

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
      return false;
    case symbol_kind:
      return _du._symbol == str_;
    case sequence_kind:
      if (_du._sequence.size() == 1)
        return front() == str_;
      else
        return false;
    case record_kind:
      return false; // TODO (wash): Requires the implementation of record. 
    default: BOOST_ASSERT("kind is only valid when masking another kind");
  }

  return false;
}

bool utree::operator== (char const* str_) const {
  if (kind() & reference_kind)
    return *_du._alias == str_;

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
      return false;
    case symbol_kind:
      return _du._symbol == str_;
    case sequence_kind:
      if (_du._sequence.size() == 1)
        return front() == str_;
      else
        return false;
    case record_kind:
      return false; // TODO (wash): Requires the implementation of record. 
    default: BOOST_ASSERT("kind is only valid when masking another kind");
  }

  return false;
}

bool utree::operator== (std::string const& str_) const {
  if (kind() & reference_kind)
    return *_du._alias == str_;

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
      return false;
    case symbol_kind:
      return _du._symbol == str_;
    case sequence_kind:
      if (_du._sequence.size() == 1)
        return front() == str_;
      else
        return false;
    case record_kind:
      return false; // TODO (wash): Requires the implementation of record. 
    default: BOOST_ASSERT("kind is only valid when masking another kind");
  }

  return false;
}

template<typename RHS>
bool utree::operator== (RHS rhs) const {
  if (rhs.kind() & reference_kind)
    return *this == *rhs._du._alias;

  else if (kind() & reference_kind)
    return *_du._alias == rhs;

  else if (!(kind() & numeric_kind) && (kind() != rhs.kind()))
    return false;

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
      return _du._numeric == rhs._du._numeric;
    case symbol_kind:
      return _du._symbol == rhs._du._symbol;
    case sequence_kind:
      return _du._sequence == rhs._du._sequence;
    case record_kind:
      return false; // TODO (wash): Requires the implementation of record. 
    default: BOOST_ASSERT("kind is only valid when masking another kind");
  }

  return false;
}

template<typename RHS>
inline bool utree::operator!= (RHS rhs) const {
  return !operator==(rhs); 
}

template<typename Tree, typename Copy>
inline void utree::copy (Tree other, Copy c) {
  clear();
  // TODO (wash): Let's add an inline method to the du structures that returns a
  // kind by reference to sugar the long member chain here.
  // DISCUSS (wash): Or, better yet, don't use kind references to dispatch the
  // copiers. I think this is the preferable solution.
  c(*this, _du._alias._data._control[0],
    other, other._du._alias._data._control[0]);
}

inline void utree::copy (const_reference other) {
  clear();
  deep(*this, _du._alias._data._control[0],
       other, other._du._alias._data._control[0]);
}

inline void utree::copy (reference other) {
  clear();
  shallow(*this, _du._alias._data._control[0],
          other, other._du._alias._data._control[0]);
}

inline void utree::copy (bool bool_) {
  clear();
  _du._numeric._data._integer = bool_;
  _du._numeric.kind(integer_kind);
}

inline void utree::copy (char char_) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(char_); 
  _du._symbol.kind(symbol_kind);
}

inline void utree::copy (boost::intmax_t int_) {
  clear();
  _du._numeric._data._integer = int_;
  _du._numeric.kind(integer_kind);
}

inline void utree::copy (double double_) {
  clear();
  _du._numeric._data._integer = double_;
  _du._numeric.kind(floating_kind);
}

inline void utree::copy (char* str) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(str);
  _du._symbol.kind(symbol_kind);
}

inline void utree::copy (char const* str) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(str);
  _du._symbol.kind(symbol_kind);
}

inline void utree::copy (char* first, char* last) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(first, last);
  _du._symbol.kind(symbol_kind);
}

inline void utree::copy (char const* first, char const* last) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(first, last);
  _du._symbol.kind(symbol_kind);
}

inline void utree::copy (char* bits, size_type len) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(bits, bits + len);
  _du._symbol.kind(symbol_kind);
}

inline void utree::copy (char const* bits, size_type len) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(bits, bits + len);
  _du._symbol.kind(symbol_kind);
}

inline void utree::copy (std::string const& str_) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(str_.begin(), str_.end());
  _du._symbol.kind(symbol_kind);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_UTREE_HPP
