/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_SEQUENCE_ITERATOR_HPP)
#define BOOST_SPIRIT_PRANA_ADT_SEQUENCE_ITERATOR_HPP

#include <boost/cstdint.hpp>

#include <boost/noncopyable.hpp>

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/add_pointer.hpp>

#include <boost/iterator/iterator_facade.hpp>

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

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_SEQUENCE_ITERATOR_HPP
