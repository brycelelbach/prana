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

#include <boost/mpl/logical.hpp>

#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/karma_generate.hpp>
#include <boost/spirit/include/karma_numeric.hpp>

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

  ~utree (void);
 
  template<class T>
    reference operator= (T);
 
  template<class T>
    void assign (T);
  void assign (char const*, char const*);
  void assign (char const*, size_type);
  template<class T0, class T1>
    void assign (T0, T1);

  template<class Tree>
    void swap (Tree&);

  metadata kind (void) const;

  void clear (void);
  
  template<kind_type Kind>
    void become (void);

  template<class Value>
    void push_front (Value);
  template<class Value>
    void push_back (Value);
  
  void pop_front (void);
  void pop_back (void);

  template<class Value>
    bool insert (iterator, Value);

  bool erase (iterator);

  // WARN (wash): The result of --end() is undefined.

        iterator begin (void);
  const_iterator begin (void) const;

        iterator end (void);
  const_iterator end (void) const;
  
        reference front (void);
  const_reference front (void) const;

        reference back (void);
  const_reference back (void) const;

  // EXPLAIN (wash): Retrieve a numeric utree as an integral/floating point
  // type. Symbol utrees will return either a Karma (in the future,
  // construe_cast) converted value, or it'll throw/assert. Container utrees
  // will also throw/assert. 
  template<class T>
    typename enable_if<is_integral<T>, T>::type get (void) const;
  template<class T>
    typename enable_if<is_floating_point<T>, T>::type get (void) const;
  // EXPLAIN (wash): Retrieve a symbol utree as an string. If the utree is
  // a numeric, the value of the utree is converted to a string. If the utree is
  // a sequence, it's converted to a cons, aka (a, (b, (c, 0))).
  template<class T>
    typename enable_if<is_same<T, std::string>, T>::type get (void) const;
  // EXPLAIN (wash): Get a sequence utree as an arbitrary STL Container with
  // value_type utree. If the utree is not a sequence, return an instance of
  // said arbitrary STL Container with the current utree as it's only element.
  // This is only useful if you need repeated random access to a sequence utree
  // (aka enough random access that it's cheaper to copy the sequence to a
  // dynamic array first). This will be used for the utree <-> client AST
  // translation framework. 
  template<class T>
    typename enable_if<
      mpl::and_<is_class<T>, mpl::not_<is_same<std::string, T> > >, T
    >::type get (void) const;

  template<class RHS>
    bool operator== (RHS) const;
  template<class RHS>
    bool operator!= (RHS) const;

 private:
  friend struct shallow_copy;
  friend struct deep_copy;
  friend struct visitor;
  
  bool equal (bool) const;
  bool equal (char) const;
  template<class RHS>
    typename enable_if<is_integral<RHS>, bool>::type equal (RHS) const;
  template<class RHS>
    typename enable_if<is_floating_point<RHS>, bool>::type equal (RHS) const;
  bool equal (char*) const;
  bool equal (char const*) const;
  bool equal (std::string const&) const;
  template<class RHS>
    typename enable_if<is_class<RHS>, bool>::type equal (RHS) const;

  template<typename Tree, typename Copy>
    void copy (Tree, Copy);
  void copy (const_reference);
  void copy (reference);
  void copy (bool);
  void copy (char);
  template<class RHS>
    typename enable_if<is_integral<RHS>, void>::type copy (RHS);
  template<class RHS>
    typename enable_if<is_floating_point<RHS>, void>::type copy (RHS);
  void copy (char*);
  void copy (char const*);
  void copy (char*, char*);
  void copy (char const*, char const*);
  void copy (char*, size_type);
  void copy (char const*, size_type);
  void copy (std::string const&);
  template<typename Iterator>
    typename enable_if<
      is_same<typename Iterator::value_type, char>, void
    >::type copy (Iterator, Iterator);
 
  discriminated_union _du;
};

utree::utree (void) {
  std::memset(&_du, 0, sizeof(discriminated_union));
  _du._alias.kind(nil_kind); 
}

template<class T>
utree::utree (T t) {
  copy(t);
}

template<class T0, class T1>
utree::utree (T0 t0, T1 t1) {
  copy(t0, t1);
}

utree::~utree (void) {
  clear();
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

void utree::assign (char const* first_, char const* last_) {
  if (kind() & reference_kind)
    return _du._alias->assign(first_, last_);

  if (kind() != symbol_kind)
    copy(first_, last_);

  // DISCUSS (wash): Is this more expensive than copying an identical symbol?
  if (kind() == symbol_kind && !std::equal(first_, last_, _du._symbol.begin()))
    copy(first_, last_); 
}

void utree::assign (char const* bits_, size_type len_) {
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
  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
      return _du._numeric.clear();
    case symbol_kind:
      return _du._symbol.clear();
    case sequence_kind:
      return _du._sequence.clear();
    case record_kind: // TODO (wash): Requires the implementation of record.
      return;
    case reference_kind:
      return _du._alias.clear();
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      return;
  }
}

template<>
inline void utree::become<nil_kind> (void) {
  clear();
  _du._alias.kind(nil_kind);
}

template<>
inline void utree::become<integer_kind> (void) {
  clear();
  _du._numeric.kind(integer_kind);
}

template<>
inline void utree::become<floating_kind> (void) {
  clear();
  _du._numeric.kind(floating_kind);
}

template<>
inline void utree::become<symbol_kind> (void) {
  clear();
  _du._symbol.kind(symbol_kind);
}

template<>
inline void utree::become<sequence_kind> (void) {
  clear();
  _du._symbol.kind(sequence_kind);
}

template<class Value>
void utree::push_front (Value val) {
  if (kind() & reference_kind)
    return _du._alias->push_front(val);

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
    case record_kind:
    case symbol_kind: {
      utree old(*this, deep);
      become<sequence_kind>();
      _du._sequence.push_front(old);
      _du._sequence.push_front(val);
      return;
    }
    case sequence_kind:
      _du._sequence.push_front(val);
      return;
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      return;
  } 
}

template<class Value>
void utree::push_back (Value val) {
  if (kind() & reference_kind)
    return _du._alias->push_back(val);

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
    case record_kind:
    case symbol_kind: {
      utree old(*this, deep);
      become<sequence_kind>();
      _du._sequence.push_back(old);
      _du._sequence.push_back(val);
      return;
    }
    case sequence_kind:
      _du._sequence.push_back(val);
      return; 
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      return;
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
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      return;
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
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      return;
  } 
}

template<class Value>
inline bool utree::insert (iterator pos, Value val) {
  if (kind() & reference_kind)
    return _du._alias->insert(pos, val);

  else if (kind() == sequence_kind) {
    _du._sequence.insert(pos, val);
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
    return iterator(this);
}

inline utree::const_iterator utree::begin (void) const {
  if (kind() & reference_kind)
    return _du._alias->begin();

  else if (kind() == sequence_kind)
    return _du._sequence.begin();

  else
    return const_iterator(this);
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

template<class T>
inline typename enable_if<is_integral<T>, T>::type utree::get (void) const {
  if (kind() & reference_kind)
    return _du._alias->template get<T>();

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
      return _du._numeric.template get<T>();
    case symbol_kind: {
      char const* first = _du._symbol.begin();
      char const* last = _du._symbol.end();
      T temp;
      qi::int_parser<boost::intmax_t> intmax_t_; 

      if (qi::parse(first, last, intmax_t_, temp))
        return temp;
      else {
        BOOST_ASSERT("cannot convert string utree to floating point type");
        break; 
      }
    }
    case sequence_kind:
      BOOST_ASSERT("cannot convert sequence utree to floating point type");
      break;
    case record_kind:
      break; // TODO (wash): Requires the implementation of record.
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      break;
  }
  return T();
}

template<class T>
inline typename enable_if<is_floating_point<T>, T>::type
utree::get (void) const {
  if (kind() & reference_kind)
    return _du._alias->template get<T>();

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
      return _du._numeric.template get<T>();
    case symbol_kind: {
      char const* first = _du._symbol.begin();
      char const* last = _du._symbol.end();
      T temp;

      if (qi::parse(first, last, qi::double_, temp))
        return temp;
      else {
        BOOST_ASSERT("cannot convert string utree to floating point type");
        break;
      }
    }
    case sequence_kind:
      BOOST_ASSERT("cannot convert sequence utree to floating point type"); 
      break;
    case record_kind:
      break; // TODO (wash): Requires the implementation of record. 
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      break;
  }
  return T();
}

template<class T>
inline typename enable_if<is_same<T, std::string>, T>::type
utree::get (void) const {
  if (kind() & reference_kind)
    return _du._alias->template get<T>();

  switch ((kind_type) kind()) {
    case nil_kind:
      return "0";
    case integer_kind: {
      T temp;
      std::back_insert_iterator<T> sink(temp);
      karma::int_generator<boost::intmax_t> intmax_t_;
      karma::generate(sink, intmax_t_, _du._numeric.get<boost::intmax_t>());
      return temp;
    }
    case floating_kind: {
      T temp;
      std::back_insert_iterator<T> sink(temp);
      karma::generate(sink, karma::double_, _du._numeric.get<double>());
      return temp;
    }
    case symbol_kind:
      return _du._symbol.str();
    case sequence_kind: {
      T temp = "(";
      for (const_iterator it = begin(), last = end(); it != last;) {
        temp += it->template get<T>();
        if (++it != last) temp += ", ";
      }
      temp += ")";
      return temp;
    }
    case record_kind:
      break; // TODO (wash): Requires the implementation of record. 
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      break;
  }
  return T();
}

template<class T>
typename enable_if<
  mpl::and_<is_class<T>, mpl::not_<is_same<std::string, T> > >, T
>::type utree::get (void) const {
  if (kind() & reference_kind)
    return _du._alias->template get<T>();

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
    case symbol_kind:
    case record_kind: {
      T temp;
      temp.push_back(*this);
      return temp;
    }
    case sequence_kind:
      return _du._sequence.template get<T>();
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      break;
  }
  return T();
}

template<typename RHS>
inline bool utree::operator== (RHS rhs_) const {
  return equal(rhs_); 
}

template<typename RHS>
inline bool utree::operator!= (RHS rhs_) const {
  return !equal(rhs_); 
}

bool utree::equal (bool bool_) const {
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
      break; // TODO (wash): Requires the implementation of record. 
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      break;
  }
  return false;
}

bool utree::equal (char char_) const {
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
      break; // TODO (wash): Requires the implementation of record. 
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      break;
  }
  return false;
}

template<typename RHS>
inline typename enable_if<is_integral<RHS>, bool>::type
utree::equal (RHS rhs_) const {
  if (kind() & reference_kind)
    return *_du._alias == rhs_;

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
      return _du._numeric == rhs_;
    case symbol_kind:
      return false;
    case sequence_kind:
      if (_du._sequence.size() == 1)
        return front() == rhs_;
      else
        return false;
    case record_kind:
      break;  // TODO (wash): Requires the implementation of record. 
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      break;
  }
  return false;
}

template<typename RHS>
inline typename enable_if<is_floating_point<RHS>, bool>::type
utree::equal (RHS rhs_) const {
  if (kind() & reference_kind)
    return *_du._alias == rhs_;

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
      return _du._numeric == rhs_;
    case symbol_kind:
      return false;
    case sequence_kind:
      if (_du._sequence.size() == 1)
        return front() == rhs_;
      else
        return false;
    case record_kind:
      break; // TODO (wash): Requires the implementation of record. 
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      break;
  }
  return false;
}

bool utree::equal (char* str_) const {
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
      break; // TODO (wash): Requires the implementation of record. 
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      break;
  }
  return false;
}

bool utree::equal (char const* str_) const {
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
      break; // TODO (wash): Requires the implementation of record. 
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      break;
  }
  return false;
}

bool utree::equal (std::string const& str_) const {
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
      break; // TODO (wash): Requires the implementation of record. 
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      break;
  }
  return false;
}

template<typename RHS>
inline typename enable_if<is_class<RHS>, bool>::type
utree::equal (RHS rhs_) const {
  if (rhs_.kind() & reference_kind)
    return *this == *rhs_._du._alias;

  else if (kind() & reference_kind)
    return *_du._alias == rhs_;

  else if (!(kind() & numeric_kind) && (kind() != rhs_.kind()))
    return false;

  switch ((kind_type) kind()) {
    case nil_kind:
    case integer_kind:
    case floating_kind:
      return _du._numeric == rhs_._du._numeric;
    case symbol_kind:
      return _du._symbol == rhs_._du._symbol;
    case sequence_kind:
      return _du._sequence == rhs_._du._sequence;
    case record_kind:
      break; // TODO (wash): Requires the implementation of record. 
    default:
      BOOST_ASSERT("kind is only valid when masking another kind");
      break;
  }
  return false;
}

template<typename Tree, typename Copy>
inline void utree::copy (Tree other_, Copy c_) {
  // TODO (wash): Let's add an inline method to the du structures that returns a
  // kind by reference to sugar the long member chain here.
  // DISCUSS (wash): Or, better yet, don't use kind references to dispatch the
  // copiers. I think this is the preferable solution.
  clear();
  c_(*this, other_);
}

inline void utree::copy (const_reference other_) {
  clear();
  deep(*this, other_);
}

inline void utree::copy (reference other_) {
  clear();
  shallow(*this, other_);
}

inline void utree::copy (bool bool_) {
  clear();
  _du._numeric.copy(bool_);
}

inline void utree::copy (char char_) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(char_); 
}

template<typename RHS>
inline typename enable_if<is_integral<RHS>, void>::type
utree::copy (RHS rhs_) {
  clear();
  _du._numeric.copy(rhs_);
}

template<typename RHS>
inline typename enable_if<is_floating_point<RHS>, void>::type
utree::copy (RHS rhs_) {
  clear();
  _du._numeric.copy(rhs_);
}

inline void utree::copy (char* str) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(str);
}

inline void utree::copy (char const* str) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(str);
}

inline void utree::copy (char* first, char* last) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(first, last);
}

inline void utree::copy (char const* first, char const* last) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(first, last);
}

inline void utree::copy (char* bits, size_type len) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(bits, bits + len);
}

inline void utree::copy (char const* bits, size_type len) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(bits, bits + len);
}

inline void utree::copy (std::string const& str_) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(str_.begin(), str_.end());
}

template<typename Iterator>
inline typename enable_if<
  is_same<typename Iterator::value_type, char>, void
>::type utree::copy (Iterator first_, Iterator last_) {
  clear();
  _du._symbol.default_construct();
  _du._symbol.deep_copy(first_, last_);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_UTREE_HPP

