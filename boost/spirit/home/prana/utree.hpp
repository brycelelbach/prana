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

#include <string>

#include <boost/ref.hpp>
#include <boost/assert.hpp>

#include <boost/spirit/home/prana/adt/dllist.hpp>
#include <boost/spirit/home/prana/adt/typed_string.hpp>
#include <boost/spirit/home/prana/adt/fast_string.hpp>
#include <boost/spirit/home/prana/adt/any_ptr.hpp>
#include <boost/spirit/home/prana/adt/irange.hpp>

#include <boost/spirit/home/prana/constructs/record.hpp>

#include <boost/spirit/home/prana/functional/index.hpp>
#include <boost/spirit/home/prana/functional/visit.hpp>

#include <boost/spirit/home/prana/tree_type.hpp>
#include <boost/spirit/home/prana/shallow.hpp>

namespace boost {
namespace spirit {
namespace prana {

class utree {
 public:
  typedef utree value_type;
  typedef dllist<utree>::iterator iterator;
  typedef dllist<utree>::const_iterator const_iterator;
  typedef dllist<utree>::ref_iterator ref_iterator;
  typedef utree& reference;
  typedef utree const& const_reference;
  typedef std::ptrdiff_t difference_type;
  typedef std::size_t size_type;

  typedef irange<iterator> range;
  typedef irange<const_iterator> const_range;

  utree (void);
  utree (bool b);
  utree (char c);
  utree (unsigned int i);
  utree (int i);
  utree (double d);
  utree (char const* str);
  utree (char const* str, std::size_t len);
  utree (std::string const& str);
  utree (boost::reference_wrapper<utree> ref);
  utree (any_ptr const& p);
  utree (range r, shallow_tag);
  utree (const_range r, shallow_tag);
  utree (utf8_string_range const& str, shallow_tag);
  utree (record<utree>* pf);
  utree (const_reference other);

  template<template<typename> class Range, typename Iter>
  utree (Range<Iter> r);

  template<typename Base, tree_type::info type_>
  utree (typed_string<Base, type_> const& str);

  ~utree (void);

  reference operator= (const_reference other);
  reference operator= (bool b);
  reference operator= (unsigned int i);
  reference operator= (int i);
  reference operator= (double d);
  reference operator= (char const* s);
  reference operator= (std::string const& s);
  reference operator= (boost::reference_wrapper<utree> ref);
  reference operator= (record<utree>* pf_);

  template<template<typename> class Range, typename Iter>
  reference operator= (Range<Iter> r);

  template<typename Base, tree_type::info type_>
  reference operator= (typed_string<Base, type_> const& bin);

  template<typename T>
  void push_back (T const& val);

  template<typename T>
  void push_front (T const& val);

  template<typename T>
  iterator insert (iterator pos, T const& x);

  template<typename T>
  void insert (iterator pos, size_type n, T const& x);

  template<typename Iter>
  void insert (iterator pos, Iter first, Iter last);

  template<typename Iter>
  void assign (Iter first, Iter last);

  void clear (void);

  void pop_front (void);
  void pop_back (void);

  iterator erase (iterator pos);
  iterator erase (iterator first, iterator last);

  reference front (void);
  reference back (void);
  const_reference front (void) const;
  const_reference back (void) const;

  reference operator[] (size_type i);
  const_reference operator[] (size_type i) const;

  void swap (utree& other);

  iterator begin (void);
  iterator end (void);
  const_iterator begin (void) const;
  const_iterator end (void) const;

  ref_iterator ref_begin (void);
  ref_iterator ref_end (void);

  bool empty (void) const;
  size_type size (void) const;

  tree_type::info which (void) const;

  reference deref (void);
  const_reference deref (void) const;

  short tag (void) const;
  void tag (short tag);

  utree eval (environment<utree> const& env) const;

 private:
  friend struct visitor;
  friend struct indexer;

  tree_type::info get_type (void) const;
  void set_type (tree_type::info t);

  void ensure_list_type (void);

  void free (void);

  void copy (const_reference other);

  union {
    fast_string<dllist<utree> > s;
    dllist<utree> l;
    irange<dllist<utree>::node*> r;
    irange<char const*> sr;
    any_ptr v;
    bool b;
    int i;
    double d;
    utree* p;
    record<utree>* pf;
  };
};


inline utree::utree (void) {
  set_type(tree_type::nil_type);
}

inline utree::utree (bool b): b(b) {
  set_type(tree_type::bool_type);
}

inline utree::utree (char c) {
  // char constructs a single element string
  s.construct(&c, &c+1);
  set_type(tree_type::string_type);
}

inline utree::utree (unsigned int i): i(i) {
  set_type(tree_type::int_type);
}

inline utree::utree (int i): i(i) {
  set_type(tree_type::int_type);
}

inline utree::utree (double d): d(d) {
  set_type(tree_type::double_type);
}

inline utree::utree (char const* str) {
  s.construct(str, str + strlen(str));
  set_type(tree_type::string_type);
}

inline utree::utree (char const* str, size_type len) {
  s.construct(str, str + len);
  set_type(tree_type::string_type);
}

inline utree::utree (std::string const& str) {
  s.construct(str.begin(), str.end());
  set_type(tree_type::string_type);
}

template<typename Base, tree_type::info type_>
inline utree::utree (typed_string<Base, type_> const& bin) {
  s.construct(bin.begin(), bin.end());
  set_type(type_);
}

inline utree::utree (boost::reference_wrapper<utree> ref): p(ref.get_pointer()) {
  set_type(tree_type::reference_type);
}

inline utree::utree (any_ptr const& p) {
  v = p;
  set_type(tree_type::any_type);
}

inline utree::utree (record<utree>* pf): pf(pf) {
  set_type(tree_type::record_type);
}

// deep copy
template<template<typename> class Range, typename Iter>
inline utree::utree (Range<Iter> r) {
  set_type(tree_type::nil_type);
  assign(r.begin(), r.end());
}

// shallow copy
inline utree::utree (range r, shallow_tag) {
  this->r.first = r.begin().node;
  this->r.last = r.end().prev;
  set_type(tree_type::range_type);
}

// shallow copy
inline utree::utree (const_range r, shallow_tag) {
  this->r.first = r.begin().node;
  this->r.last = r.end().prev;
  set_type(tree_type::range_type);
}

inline utree::utree (utf8_string_range const& str, shallow_tag) {
  this->sr.first = str.begin();
  this->sr.last = str.end();
  set_type(tree_type::string_range_type);
}

inline utree::utree (const_reference other) {
  copy(other);
}

inline utree::~utree (void) {
  free();
}

inline utree& utree::operator= (const_reference other) {
  if (this != &other) {
    free();
    copy(other);
  }
  return *this;
}

inline utree& utree::operator= (bool b_) {
  free();
  b = b_;
  set_type(tree_type::bool_type);
  return *this;
}

inline utree& utree::operator= (unsigned int i_) {
  free();
  i = i_;
  set_type(tree_type::int_type);
  return *this;
}

inline utree& utree::operator= (int i_) {
  free();
  i = i_;
  set_type(tree_type::int_type);
  return *this;
}

inline utree& utree::operator= (double d_) {
  free();
  d = d_;
  set_type(tree_type::double_type);
  return *this;
}

inline utree& utree::operator= (char const* s_) {
  free();
  s.construct(s_, s_ + strlen(s_));
  set_type(tree_type::string_type);
  return *this;
}

inline utree& utree::operator= (std::string const& s_) {
  free();
  s.construct(s_.begin(), s_.end());
  set_type(tree_type::string_type);
  return *this;
}

template<typename Base, tree_type::info type_>
inline utree& utree::operator= (typed_string<Base, type_> const& bin) {
  free();
  s.construct(bin.begin(), bin.end());
  set_type(type_);
  return *this;
}

inline utree& utree::operator= (boost::reference_wrapper<utree> ref) {
  free();
  p = ref.get_pointer();
  set_type(tree_type::reference_type);
  return *this;
}

inline utree& utree::operator= (record<utree>* pf_) {
  free();
  pf = pf_;
  set_type(tree_type::record_type);
  return *this;
}
 
template<template<typename> class Range, typename Iter>
inline utree& utree::operator= (Range<Iter> r) {
  free();
  assign(r.begin(), r.end());
  return *this;
}

template<typename T>
inline void utree::push_front (T const& val) {
  if (get_type() == tree_type::reference_type)
    return p->push_front(val);
  ensure_list_type();
  l.push_front(val);
}

template<typename T>
inline void utree::push_back (T const& val) {
  if (get_type() == tree_type::reference_type)
    return p->push_back(val);
  ensure_list_type();
  l.push_back(val);
}

template<typename T>
inline utree::iterator utree::insert (iterator pos, T const& val) {
  if (get_type() == tree_type::reference_type)
    return p->insert(pos, val);
  ensure_list_type();

  if (pos == end()) {
    push_back(val);
    return begin();
  }

  l.insert(val, pos);
  return iterator(pos.node->prev, pos.node->prev->prev);
}

template<typename T>
inline void utree::insert (iterator pos, size_type n, T const& val) {
  if (get_type() == tree_type::reference_type)
    return p->insert(pos, n, val);
  for (size_type i = 0; i != n; ++i)
    insert(pos, val);
}

template<typename Iter>
inline void utree::insert (iterator pos, Iter first, Iter last) {
  if (get_type() == tree_type::reference_type)
    return p->insert(pos, first, last);
  ensure_list_type();
  while (first != last)
    insert(pos, *first++);
}

template<typename Iter>
inline void utree::assign (Iter first, Iter last) {
  if (get_type() == tree_type::reference_type)
    return p->assign(first, last);
  ensure_list_type();
  clear();
  while (first != last) {
    push_back(*first);
    ++first;
  }
}

inline void utree::clear (void) {
  if (get_type() == tree_type::reference_type)
    return p->clear();
  free(); // clear will always make this a nil type
  set_type(tree_type::nil_type);
}

inline void utree::pop_front (void) {
  if (get_type() == tree_type::reference_type)
    return p->pop_front();
  BOOST_ASSERT(get_type() == tree_type::list_type);
  l.pop_front();
}

inline void utree::pop_back (void) {
  if (get_type() == tree_type::reference_type)
    return p->pop_back();
  BOOST_ASSERT(get_type() == tree_type::list_type);
  l.pop_back();
}

inline utree::iterator utree::erase (iterator pos) {
  if (get_type() == tree_type::reference_type)
    return p->erase(pos);
  BOOST_ASSERT(get_type() == tree_type::list_type);
  return l.erase(pos);
}

inline utree::iterator utree::erase (iterator first, iterator last) {
  if (get_type() == tree_type::reference_type)
    return p->erase(first, last);
  while (first != last)
    erase(first++);
  return last;
}

inline utree::iterator utree::begin (void) {
  if (get_type() == tree_type::reference_type)
    return p->begin();
  else if (get_type() == tree_type::range_type)
    return iterator(r.first, 0);

  // otherwise...
  ensure_list_type();
  return iterator(l.first, 0);
}

inline utree::iterator utree::end (void) {
  if (get_type() == tree_type::reference_type)
    return p->end();
  else if (get_type() == tree_type::range_type)
    return iterator(0, r.first);

  // otherwise...
  ensure_list_type();
  return iterator(0, l.last);
}

inline utree::ref_iterator utree::ref_begin (void) {
  if (get_type() == tree_type::reference_type)
    return p->ref_begin();
  else if (get_type() == tree_type::range_type)
    return ref_iterator(r.first, 0);

  // otherwise...
  ensure_list_type();
  return ref_iterator(l.first, 0);
}

inline utree::ref_iterator utree::ref_end (void) {
  if (get_type() == tree_type::reference_type)
    return p->ref_end();
  else if (get_type() == tree_type::range_type)
    return ref_iterator(0, r.first);

  // otherwise...
  ensure_list_type();
  return ref_iterator(0, l.last);
}

inline utree::const_iterator utree::begin (void) const {
  if (get_type() == tree_type::reference_type)
    // FIXME: C style cast is bad
    return ((utree const*)p)->begin();
  else if (get_type() == tree_type::range_type)
    return const_iterator(r.first, 0);

  // otherwise...
  BOOST_ASSERT(get_type() == tree_type::list_type);
  return const_iterator(l.first, 0);
}

inline utree::const_iterator utree::end (void) const {
  if (get_type() == tree_type::reference_type)
    // FIXME: C style cast is bad
    return ((utree const*)p)->end();
  else if (get_type() == tree_type::range_type)
    return const_iterator(0, r.first);

  // otherwise...
  BOOST_ASSERT(get_type() == tree_type::list_type);
  return const_iterator(0, l.last);
}

inline bool utree::empty (void) const {
  if (get_type() == tree_type::reference_type)
    // FIXME: C style cast is bad
    return ((utree const*)p)->empty();
  else if (get_type() == tree_type::range_type)
    return r.first == 0;
  else if (get_type() == tree_type::list_type)
    return l.size == 0;
  return get_type() == tree_type::nil_type;
}

inline std::size_t utree::size (void) const {
  if (get_type() == tree_type::reference_type)
    // FIXME: C style cast is bad
    return ((utree const*)p)->size();
  else if (get_type() == tree_type::range_type) {
    size_type size = 0;
    dllist<utree>::node* n = r.first;
    while (n) {
      n = n->next;
      ++size;
    }
    return size;
  }
  else if (get_type() == tree_type::list_type)
    return l.size;
  
  BOOST_ASSERT(get_type() == tree_type::nil_type);
  return 0;
}

inline tree_type::info utree::which (void) const {
  return get_type();
}

inline utree& utree::front (void) {
  if (get_type() == tree_type::reference_type)
    return p->front();
  else if (get_type() == tree_type::range_type) {
    BOOST_ASSERT(r.first != 0);
    return r.first->val;
  }

  // otherwise...
  BOOST_ASSERT(get_type() == tree_type::list_type && l.first != 0);
  return l.first->val;
}

inline utree& utree::back (void) {
  if (get_type() == tree_type::reference_type)
    return p->back();
  else if (get_type() == tree_type::range_type) {
    BOOST_ASSERT(r.last != 0);
    return r.last->val;
  }

  // otherwise...
  BOOST_ASSERT(get_type() == tree_type::list_type && l.last != 0);
  return l.last->val;
}

inline utree const& utree::front (void) const {
  if (get_type() == tree_type::reference_type)
    // FIXME: C style cast is bad
    return ((utree const*)p)->front();
  else if (get_type() == tree_type::range_type) {
    BOOST_ASSERT(r.first != 0);
    return r.first->val;
  }

  // otherwise...
  BOOST_ASSERT(get_type() == tree_type::list_type && l.first != 0);
  return l.first->val;
}

inline utree const& utree::back (void) const {
  if (get_type() == tree_type::reference_type)
    // FIXME: C style cast is bad
    return ((utree const*)p)->back();
  else if (get_type() == tree_type::range_type) {
    BOOST_ASSERT(r.last != 0);
    return r.last->val;
  }

  // otherwise...
  BOOST_ASSERT(get_type() == tree_type::list_type && l.last != 0);
  return l.last->val;
}

inline utree& utree::operator[] (size_type i) {
  if (get_type() == tree_type::reference_type)
    return (*p)[i];
  else if (get_type() == tree_type::range_type)
    return index(r.first, i);

  // otherwise...
  BOOST_ASSERT(get_type() == tree_type::list_type && size() > i);
  return index(l.first, i);
}

inline utree const& utree::operator[] (size_type i) const {
  if (get_type() == tree_type::reference_type)
    // FIXME: C style cast is bad
    return (*(utree const*)p)[i];
  else if (get_type() == tree_type::range_type)
    return index(r.first, i);

  // otherwise...
  BOOST_ASSERT(get_type() == tree_type::list_type && size() > i);
  return index(l.first, i);
}

inline void utree::swap (reference other) {
  s.swap(other.s);
}

inline tree_type::info utree::get_type (void) const {
  // the fast string holds the type info
  return static_cast<tree_type::info>(s.get_type());
}

inline void utree::set_type (tree_type::info t) {
  // the fast string holds the type info
  s.set_type(t);
}

inline void utree::ensure_list_type (void) {
  if (get_type() == tree_type::nil_type) {
    set_type(tree_type::list_type);
    l.default_construct();
  }

  else
    BOOST_ASSERT(get_type() == tree_type::list_type);
}

inline void utree::free (void) {
  switch (get_type()) {
    case tree_type::binary_type:
    case tree_type::symbol_type:
    case tree_type::string_type:
      s.free();
      break;
    case tree_type::list_type:
      l.free();
      break;
    case tree_type::record_type:
      if (pf) delete pf;
      pf = 0;
      break;
    default:
      break;
  };
}

inline void utree::copy (const_reference other) {
  set_type(other.get_type());
  switch (other.get_type()) {
    case tree_type::nil_type:
      break;
    case tree_type::bool_type:
      b = other.b;
      break;
    case tree_type::int_type:
      i = other.i;
      break;
    case tree_type::double_type:
      d = other.d;
      break;
    case tree_type::reference_type:
      p = other.p;
      break;
    case tree_type::any_type:
      v = other.v;
      break;
    case tree_type::range_type:
      r = other.r;
      break;
    case tree_type::string_range_type:
      sr = other.sr;
      break;
    case tree_type::record_type:
      pf = other.pf->clone();
      break;
    case tree_type::string_type:
    case tree_type::symbol_type:
    case tree_type::binary_type:
      s.copy(other.s);
      break;
    case tree_type::list_type:
      l.copy(other.l);
      s.tag(other.s.tag());
      break;
  }
}

inline utree& utree::deref (void) {
  return (get_type() == tree_type::reference_type) ? *p : *this;
}

inline utree const& utree::deref (void) const {
  return (get_type() == tree_type::reference_type) ? *p : *this;
}

inline short utree::tag (void) const {
  BOOST_ASSERT(get_type() == tree_type::list_type);
  return s.tag();
}

inline void utree::tag (short tag) {
  ensure_list_type();
  s.tag(tag);
}

inline utree utree::eval (environment<utree> const& env) const {
  BOOST_ASSERT(get_type() == tree_type::record_type);
  return (*pf)(env);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_UTREE_HPP
