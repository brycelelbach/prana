/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_SEQUENCE_HPP)
#define BOOST_SPIRIT_PRANA_ADT_SEQUENCE_HPP

#include <cstring>
#include <climits>

#include <algorithm>

#include <boost/integer.hpp>
#include <boost/cstdint.hpp>

#include <boost/noncopyable.hpp>

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/add_pointer.hpp>

#include <boost/iterator/iterator_facade.hpp>

#include <boost/spirit/home/prana/kind.hpp>
#include <boost/spirit/home/prana/tag.hpp>
#include <boost/spirit/home/prana/fn/copy.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Data>
struct sequence_node: private boost::noncopyable {
  typedef Data        value_type;
  typedef Data&       reference;
  typedef Data const& const_reference;
  typedef Data*       pointer;
  typedef Data const* const_pointer;
  typedef std::size_t size_type;

  sequence_node (sequence_node*, sequence_node*, shallow_copy);
  sequence_node (sequence_node*, sequence_node*, deep_copy);
  template<typename Value>
    sequence_node (Value* val, sequence_node*, sequence_node*, shallow_copy);
  template<typename Value>
    sequence_node (Value* val, sequence_node*, sequence_node*, deep_copy);

  ~sequence_node (void);

  void unlink (void);

  bool is_shallow (void) const;

  Data* ptr (void) const;

  Data* val;
  sequence_node* next;
  sequence_node* prev;
};

template<typename Data>
sequence_node<Data>::sequence_node (
  sequence_node* other, sequence_node* prev_, shallow_copy copy
):
  val(other->val),
  next((other->next ? new sequence_node(other->next, this, copy) : 0)),
  prev(prev_)
{
  // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
}

template<typename Data>
sequence_node<Data>::sequence_node (
  sequence_node* other, sequence_node* prev_, deep_copy copy
):
  val((other->val ? new Data(*other->val) : 0)),
  next((other->next ? new sequence_node(other->next, this, copy) : 0)),
  prev(prev_)
{
  // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
}

template<typename Data>
template<typename Value>
sequence_node<Data>::sequence_node (
  Value* val_, sequence_node* next_, sequence_node* prev_, shallow_copy copy
):
  val(val_), next(next_), prev(prev_) {
    val = (Data*) (((uintptr_t) val) + 1); 
  }

template<typename Data>
template<typename Value>
sequence_node<Data>::sequence_node (
  Value* val_, sequence_node* next_, sequence_node* prev_, deep_copy deep
):
  val((val_ ? new Data(*val_) : 0)), next(next_), prev(prev_) {
    // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
  }

template<typename Data>
sequence_node<Data>::~sequence_node (void) {
  if (val && !is_shallow()) delete val;
}

template<typename Data>
inline void sequence_node<Data>::unlink (void) {
  prev->next = next;
  next->prev = prev;
}

template<typename Data>
inline bool sequence_node<Data>::is_shallow (void) const {
  return bool(((uintptr_t) val) & 1); 
}

template<typename Data>
inline Data* sequence_node<Data>::ptr (void) const {
  if (is_shallow()) return (Data*) (((uintptr_t) val) + 1);
  else return val;
}

template<typename Data>
struct sequence_iterator: public iterator_facade<
  sequence_iterator<Data>, typename remove_const<Data>::type,
  boost::bidirectional_traversal_tag
> {
 public:
  typedef typename add_pointer<
    sequence_node<typename remove_const<Data>::type>
  >::type node_type;

  sequence_iterator (void);
  sequence_iterator (node_type, node_type);
  template<typename Iterator>
    sequence_iterator (Iterator const&);

  template<typename Iterator>
    sequence_iterator& operator= (Iterator const&);

  void increment (void);
  void decrement (void);

  template<typename Iterator>
    bool equal (Iterator const&) const;
  template<typename Iterator>
    bool operator== (Iterator const&) const;
  template<typename Iterator>
    bool operator!= (Iterator const&) const;

  typename sequence_iterator::reference dereference (void) const;

  node_type curr;
  node_type prev;
};

template<typename Data>
sequence_iterator<Data>::sequence_iterator (void):
  curr(0), prev(0) {
    // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
  }

template<typename Data>
sequence_iterator<Data>::sequence_iterator (node_type curr_, node_type prev_):
  curr(curr_), prev(prev_) {
    // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
  }

template<typename Data>
template<typename Iterator>
sequence_iterator<Data>::sequence_iterator (Iterator const& it):
  curr(it.curr), prev(it.prev) {
    // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
  }

template<typename Data>
template<typename Iterator>
sequence_iterator<Data>&
sequence_iterator<Data>::operator= (Iterator const& it) {
  if (*this != it) {
    curr = it.curr;
    prev = it.prev;
  }
  return *this;
}

template<typename Data>
void sequence_iterator<Data>::increment (void) {
  if (curr != 0) { 
    prev = curr;
    curr = curr->next;
  }
}

template<typename Data>
void sequence_iterator<Data>::decrement (void) {
  if (prev != 0) {
    curr = prev;
    prev = prev->prev;
  }
}

template<typename Data>
template<typename Iterator>
bool sequence_iterator<Data>::equal (Iterator const& it) const {
  return curr == it.curr;
}

template<typename Data>
template<typename Iterator>
bool sequence_iterator<Data>::operator== (Iterator const& it) const {
  return curr == it.curr;
}

template<typename Data>
template<typename Iterator>
bool sequence_iterator<Data>::operator!= (Iterator const& it) const {
  return curr != it.curr;
}

template<typename Data>
typename sequence_iterator<Data>::reference
sequence_iterator<Data>::dereference (void) const {
  return *curr->val;
}

template<typename Data>
struct sequence {
 public:
  typedef Data                          value_type;
  typedef Data&                         reference;
  typedef Data const&                   const_reference;
  typedef Data*                         pointer;
  typedef Data const*                   const_pointer;
  typedef std::size_t                   size_type;
  typedef sequence_iterator<Data>       iterator;
  typedef sequence_iterator<Data const> const_iterator;
  
  typedef boost::uint8_t metadata;

  typedef sequence_node<Data> node_type; 

  struct storage {
    metadata _control [(sizeof(void*[1]) / 2)];
    boost::int_t<(sizeof(void*[1]) / 2) * CHAR_BIT>::exact _size;
    node_type* _first;
    node_type* _last;
  };

  void default_construct (void);
 
  // TODO (wash): Refactor shallow_copy and deep_copy using a common method
 
  void shallow_copy (sequence&);
  
  void deep_copy (sequence const&);
  
  void clear (void);
  
  template<typename Container> Container get (void) const;

  // TODO (wash): Insertation needs to support shallow copying! ATM, default is
  // to deep copy.

  template<typename Value, typename Iterator>
  void insert (Iterator, Value);

  template<typename Value> void push_front (Value);
  template<typename Value> void push_back (Value);

  void pop_front (void);
  void pop_back (void);

  iterator begin (void);
  const_iterator begin (void) const;

  iterator end (void);
  const_iterator end (void) const;

  template<typename Iterator>
  iterator erase (Iterator);

  template<typename Container> bool operator== (Container const&) const;
  template<typename Container> bool operator!= (Container const&) const;

  size_type size (void) const;
  
  metadata kind (void) const;
  void kind (metadata);

  storage _data;
};

template<typename Data>
inline void sequence<Data>::default_construct (void) {
  std::memset(&_data, 0, sizeof(storage));
}

template<typename Data>
inline void sequence<Data>::shallow_copy (sequence& other_) {
  // DISCUSS (wash): Do we have to clear here? Would it be more efficient to
  // reuse the memory allocated for nodes and/or data, or would the complexity
  // of those intrinsics make such semantics more trouble than they're worth?
  clear();

  // EXPLAIN (wash): This particular sequence_node ctor allocates recursively,
  // doing the copying work for us.
  // DISCUSS (wash): Is it more efficient to use a simple for loop to allocate
  // here, instead of recursing in sequence_node's ctor? Which can be optimized
  // best by the compiler?
  _data._first = new node_type(other_._data._first, 0, shallow);

  // DISCUSS (wash): Can we make the recursive allocation above take care of
  // this? This is totally inefficient.
  for (node_type* it = _data._first; it != 0; it = it->next)
    if (it->next == 0)
      _data._last = it;
  
  _data._size = other_._data._size;
}

template<typename Data>
inline void sequence<Data>::deep_copy (sequence const& other_) {
  clear();

  _data._first = new node_type(other_._data._first, 0, deep);

  for (node_type* it = _data._first; it != 0; it = it->next) 
    if (it->next == 0)
      _data._last = it;

  _data._size = other_._data._size;
}

template<typename Data>
inline void sequence<Data>::clear (void) {
  if (_data._first == _data._last) {
    if (_data._first)
      delete _data._first;
    return;
  }

  node_type* p = _data._first;
  
  while (p) {
    node_type* next = p->next;
    delete p;
    p = next;
  } 

  std::memset(&_data, 0, sizeof(storage)); 
}

template<typename Data>
template<typename Container>
inline Container sequence<Data>::get (void) const {
  return Container(begin(), end());
}

template<typename Data>
template<typename Value, typename Iterator>
inline void sequence<Data>::insert (Iterator pos_, Value val_) {
  if (!pos_.curr)
    return push_back(val_);

  node_type* new_node = new node_type(&val_, pos_.curr, pos_.curr->prev, deep);

  if (pos_.curr->prev)
    pos_.curr->prev->next = new_node;
  else
    _data._first = new_node;

  pos_.curr->prev = new_node;
  ++_data._size;
}

template<typename Data>
template<typename Value>
inline void sequence<Data>::push_front (Value val_) {
  node_type* new_node;

  if (_data._first == 0) {
    new_node = new node_type(&val_, 0, 0, deep);
    _data._first = _data._last = new_node;
    ++_data._size;
  }

  else {
    new_node = new node_type(&val_, _data._first, _data._first->prev, deep);
    _data._first->prev = new_node;
    _data._first = new_node;
    ++_data._size;
  }
}

template<typename Data>
template<typename Value>
inline void sequence<Data>::push_back (Value val_) {
  if (_data._last == 0)
    push_front(val_);

  else {
    node_type* new_node =
      new node_type(&val_, _data._last->next, _data._last, deep);
    _data._last->next = new_node;
    _data._last = new_node;
    ++_data._size;
  }
}

template<typename Data>
inline void sequence<Data>::pop_front (void) {
  if (!_data._first) return;

  else if (_data._first == _data._last) {
    delete _data._first;
    _data._first = _data._last = 0;
    --_data._size;
  }

  else {
    node_type* np = _data._first;
    _data._first = _data._first->next;
    _data._first->prev = 0;
    delete np;
    --_data._size;
  }
}

template<typename Data>
inline void sequence<Data>::pop_back (void) {
  if (!_data._first) return;

  else if (_data._first == _data._last) { 
    delete _data._first;
    _data._first = _data._last = 0;
    --_data._size;
  }

  else {
    node_type* np = _data._last;
    _data._last = _data._last->prev;
    _data._last->next = 0;
    delete np;
    --_data._size;
  }
}

template<typename Data>
inline typename sequence<Data>::iterator
sequence<Data>::begin (void) {
  return iterator(_data._first, 0);
}

template<typename Data>
inline typename sequence<Data>::const_iterator
sequence<Data>::begin (void) const {
  return const_iterator(_data._first, 0);
}

template<typename Data>
inline typename sequence<Data>::iterator
sequence<Data>::end (void) {
  return iterator(0, _data._last);
}

template<typename Data>
inline typename sequence<Data>::const_iterator
sequence<Data>::end (void) const {
  return const_iterator(0, _data._last);
}

template<typename Data>
template<typename Iterator>
inline typename sequence<Data>::iterator sequence<Data>::erase (Iterator pos_) {
  if (pos_.curr == 0)
    return Iterator(0, _data._last);

  else if (pos_.curr == _data._first) {
    pop_front();
    return Iterator(_data._first, 0);
  }

  else if (pos_.curr == _data._last) {
    pop_back();
    return Iterator(0, _data._last);
  }

  // DISCUSS (wash): Size will get decremented here, even if the node given
  // isn't in this sequence. Should we fix this? It would be expensive.
  node_type* next(pos_.curr->next);
  pos_.curr->unlink();
  delete pos_.curr;
  --_data._size;
  return Iterator(next, next->prev);
}

template<typename Data>
template<typename Container>
inline bool sequence<Data>::operator== (Container const& c_) const {
  return std::equal(begin(), end(), c_.begin());
}

template<typename Data>
template<typename Container>
inline bool sequence<Data>::operator!= (Container const& c_) const {
  return !std::equal(begin(), end(), c_.begin());
}

template<typename Data>
inline typename sequence<Data>::size_type sequence<Data>::size (void) const {
  return _data._size;
}

template<typename Data>
inline typename sequence<Data>::metadata
sequence<Data>::kind (void) const {
  return _data._control[0];
}

template<typename Data>
inline void sequence<Data>::kind (metadata kind_) {
  _data._control[0] = kind_;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_SEQUENCE_HPP
