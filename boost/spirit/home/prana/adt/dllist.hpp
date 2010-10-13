/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_DLLIST_HPP)
#define BOOST_SPIRIT_PRANA_DLLIST_HPP

#include <boost/assert.hpp>
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

  template<typename T>
  void insert_before (T const& val, node* node);

  template<typename T>
  void insert_after (T const& val, node* node);

  template<typename T>
  void push_front (T const& val);

  template<typename T>
  void push_back (T const& val);

  void pop_front (void);
  void pop_back (void);

  node* erase (node* pos);

  node* first;
  node* last;
  size_type size;
};

template<typename Data>
struct dllist<Data>::node: private boost::noncopyable {
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
  first = last = 0;
  size = 0;
}

template<typename Data>
template<typename T>
inline void dllist<Data>::insert_before (T const& val, node* np) {
  BOOST_ASSERT(np != 0);

  node* new_node = new node(val, np, np->prev);

  if (np->prev)
    np->prev->next = new_node;
  else
    first = new_node;

  np->prev = new_node;
  ++size;
}

template<typename Data>
template<typename T>
inline void dllist<Data>::insert_after (T const& val, node* np) {
  BOOST_ASSERT(np != 0);

  node* new_node = new node(val, np->next, np);

  if (np->next)
    np->next->prev = new_node;
  else
    last = new_node;

  np->next = new_node;
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

  else insert_before(val, first);
}

template<typename Data>
template<typename T>
inline void dllist<Data>::push_back(T const& val) {
  if (last == 0)
    push_front(val);
  else
    insert_after(val, last);
}

template<typename Data>
inline void dllist<Data>::pop_front (void) {
  BOOST_ASSERT(size != 0);

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
  BOOST_ASSERT(size != 0);

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
inline typename dllist<Data>::node* dllist<Data>::erase (node* pos) {
  BOOST_ASSERT(pos != 0);

  if (pos == first) {
    pop_front();
    return first;
  }

  else if (pos == last) {
    pop_back();
    return 0;
  }

  node* next(pos->next);
  pos->unlink();
  delete pos;
  --size;
  return next;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_DLLIST_HPP
