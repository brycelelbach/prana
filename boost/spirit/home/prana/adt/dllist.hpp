/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_DLLIST_HPP)
#define BOOST_SPIRIT_PRANA_DLLIST_HPP

#include <boost/ref.hpp>
#include <boost/noncopyable.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace boost {
namespace spirit {
namespace prana {

/*=============================================================================
Our POD double linked list. Straightforward implementation. This has to be a POD
so it can be used in utree's discriminated union.
=============================================================================*/

template<typename Data>
struct dllist {
 public:
  struct node;

  template<typename Value> class node_iterator;

  typedef std::size_t size_type;

  typedef node_iterator<Data>                            iterator;
  typedef node_iterator<const Data>                      const_iterator;
  typedef node_iterator<boost::reference_wrapper<Data> > ref_iterator;

  void free (void);
  void copy (dllist const& other);
  void default_construct (void);

  template<typename T, typename Iterator>
  void insert (T const& val, Iterator pos);

  template<typename T>
  void push_front (T const& val);

  template<typename T>
  void push_back (T const& val);

  void pop_front (void);
  void pop_back (void);

  iterator begin (void);
  const_iterator begin (void) const;

  iterator end (void);
  const_iterator end (void) const;

  template<typename Iterator>
  iterator erase (Iterator pos);

  node* first;
  node* last;
  size_type size;
};

template<typename Data>
struct dllist<Data>::node: private boost::noncopyable {
  typedef Data value_type;

  template<typename T>
  node (T const& val, node* next, node* prev):
    val(val), next(next), prev(prev) { }

  void unlink (void) {
    prev->next = next;
    next->prev = prev;
  }

  Data val;
  node* next;
  node* prev;
};

template<typename Data>
template<typename Value>
class dllist<Data>::node_iterator: public boost::iterator_facade<
  node_iterator<Value>, Value, boost::bidirectional_traversal_tag
> {
 public:
  node_iterator (void): node(0) { }

  node_iterator (dllist<Data>::node* node, dllist<Data>::node* prev):
    node(node), prev(prev) { }

  void increment (void) {
    if (node != 0) { // not at end
      prev = node;
      node = node->next;
    }
  }

  void decrement (void) {
    if (prev != 0) { // not at begin
      node = prev;
      prev = prev->prev;
    }
  }

  bool equal (node_iterator const& other) const {
    return node == other.node;
  }

  typename node_iterator::reference dereference (void) const {
    return node->val;
  }

  dllist<Data>::node* node;
  dllist<Data>::node* prev;
};

template<typename Data>
template<typename Value>
class dllist<Data>::node_iterator<boost::reference_wrapper<Value> >:
  public boost::iterator_facade<
    node_iterator<boost::reference_wrapper<Value> >,
    boost::reference_wrapper<Value>,
    boost::bidirectional_traversal_tag
> {
 public:
  node_iterator (void):
    node(0), prev(0), curr(nil_node) { }

  node_iterator (dllist<Data>::node* node, dllist<Data>::node* prev):
    node(node), prev(prev), curr(node ? (node->val) : nil_node) { }

  void increment (void) {
    if (node != 0) { // not at end
      prev = node;
      node = node->next;
      curr = boost::ref(node ? (node->val) : nil_node);
    }
  }

  void decrement (void) {
    if (prev != 0) { // not at begin
      node = prev;
      prev = prev->prev;
      curr = boost::ref(node ? (node->val) : nil_node);
    }
  }

  bool equal (node_iterator const& other) const {
    return node == other.node;
  }

  typename node_iterator::reference dereference (void) const {
    return curr;
  }

  dllist<Data>::node* node;
  dllist<Data>::node* prev;

  static Value nil_node;
  mutable boost::reference_wrapper<Value> curr;
};

template<typename Data>
template<typename Value>
Value dllist<Data>::node_iterator<boost::reference_wrapper<Value> >::nil_node
  = Value();

template<typename Data>
inline void dllist<Data>::free (void) {
  node* p = first;
  
  while (p != last) {
    node* next = p->next;
    delete p;
    p = next;
  }

  first = last = 0; size = 0;
}

template<typename Data>
inline void dllist<Data>::copy (dllist const& other) {
  first = last = 0; size = 0;
  node* p = other.first;

  while (p != 0) {
    push_back(p->val);
    p = p->next;
  }
}

template<typename Data>
inline void dllist<Data>::default_construct (void) {
  first = last = 0; size = 0;
}

template<typename Data>
template<typename T, typename Iterator>
inline void dllist<Data>::insert (T const& val, Iterator pos) {
  node* new_node = new node(val, pos.node, pos.node->prev);

  if (pos.node->prev)
    pos.node->prev->next = new_node;
  else
    first = new_node;

  pos.node->prev = new_node;
  ++size;
}

template<typename Data>
template<typename T>
inline void dllist<Data>::push_front (T const& val) {
  node* new_node;

  if (first == 0) {
    new_node = new node(val, 0, 0);
    first = last = new_node;
    ++size;
  }

  else {
    new_node = new node(val, first, first->prev);

    first->prev = new_node;
    first = new_node;

    ++size;
  }
}

template<typename Data>
template<typename T>
inline void dllist<Data>::push_back (T const& val) {
  if (last == 0) push_front(val);

  else {
    node* new_node = new node(val, last->next, last);

    last->next = new_node;
    last = new_node;
    
    ++size;
  }
}

template<typename Data>
inline void dllist<Data>::pop_front (void) {
  if (size == 0) return;

  if (first == last) { // there's only one item
    delete first;
    size = 0;
    first = last = 0;
  }

  else {
    node* np = first;
    first = first->next;
    first->prev = 0;
    delete np;
    --size;
  }
}

template<typename Data>
inline void dllist<Data>::pop_back (void) {
  if (size == 0) return;

  if (first == last) { // there's only one item
    delete first;
    size = 0;
    first = last = 0;
  }

  else {
    node* np = last;
    last = last->prev;
    last->next = 0;
    delete np;
    --size;
  }
}

template<typename Data>
inline typename dllist<Data>::iterator dllist<Data>::begin (void) {
  return iterator(first, 0);
}

template<typename Data>
inline typename dllist<Data>::const_iterator dllist<Data>::begin (void) const {
  return const_iterator(first, 0);
}

template<typename Data>
inline typename dllist<Data>::iterator dllist<Data>::end (void) {
  return iterator(0, last);
}

template<typename Data>
inline typename dllist<Data>::const_iterator dllist<Data>::end (void) const {
  return const_iterator(0, last);
}

template<typename Data>
template<typename Iterator>
inline typename dllist<Data>::iterator dllist<Data>::erase (Iterator pos) {
  if (pos.node == 0) return Iterator(0, last);

  if (pos.node == first) {
    pop_front();
    return Iterator(first, 0);
  }

  else if (pos.node == last) {
    pop_back();
    return Iterator(0, last);
  }

  node* next(pos.node->next);
  pos.node->unlink();
  delete pos.node;
  --size;
  return Iterator(next, next->prev);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_DLLIST_HPP
