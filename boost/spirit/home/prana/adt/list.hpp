/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_LIST_HPP)
#define BOOST_SPIRIT_PRANA_LIST_HPP

#include <boost/assert.hpp>
#include <boost/ref.hpp>
#include <boost/noncopyable.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include <boost/spirit/home/prana/utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct list::node: private boost::noncopyable {
  template<typename T>
  node (T const& val, node* next, node* prev):
    val(val), next(next), prev(prev) { }

  void unlink (void) {
    prev->next = next;
    next->prev = prev;
  }

  utree val;
  node* next;
  node* prev;
};

template<typename Value>
class list::iterator: public boost::iterator_facade<
  iterator<Value>, Value, boost::bidirectional_traversal_tag
> {
 public:
  iterator (void): node(0) { }

  iterator (list::node* node, list::node* prev): node(node), prev(prev) { }

 private:
  friend class boost::iterator_core_access;
  friend class utree;

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

  bool equal (iterator const& other) const { return node == other.node; }

  typename iterator::reference dereference (void) const { return node->val; }

  list::node* node;
  list::node* prev;
};

template<typename Value>
class list::iterator<boost::reference_wrapper<Value> >:
  public boost::iterator_facade<
    iterator<boost::reference_wrapper<Value> >,
    boost::reference_wrapper<Value>,
    boost::bidirectional_traversal_tag>
{
 public:
  iterator (void):
    node(0), prev(0), curr(nil_node) { }

  iterator (list::node* node, list::node* prev):
    node(node), prev(prev), curr(node ? (node->val) : nil_node) { }

 private:
  friend class boost::iterator_core_access;
  friend class utree;

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

  bool equal (iterator const& other) const { return node == other.node; }

  typename iterator::reference dereference (void) const { return curr; }

  list::node* node;
  list::node* prev;

  static Value nil_node;
  mutable boost::reference_wrapper<Value> curr;
};

template<typename Value>
Value list::iterator<boost::reference_wrapper<Value> >::nil_node = Value();

inline void list::free (void) {
  node* p = first;
  
  while (p != last) {
    node* next = p->next;
    delete p;
    p = next;
  }

  first = last = 0; size = 0;
}

inline void list::copy (list const& other) {
  first = last = 0; size = 0;
  node* p = other.first;

  while (p != 0) {
    push_back(p->val);
    p = p->next;
  }
}

inline void list::default_construct (void) {
  first = last = 0;
  size = 0;
}

template<typename T>
inline void list::insert_before (T const& val, node* np) {
  BOOST_ASSERT(np != 0);

  node* new_node = new node(val, np, np->prev);

  if (np->prev)
    np->prev->next = new_node;
  else
    first = new_node;

  np->prev = new_node;
  ++size;
}

template <typename T>
inline void list::insert_after (T const& val, node* np) {
  BOOST_ASSERT(np != 0);

  node* new_node = new node(val, np->next, np);

  if (np->next)
    np->next->prev = new_node;
  else
    last = new_node;

  np->next = new_node;
  ++size;
}

template<typename T>
inline void list::push_front (T const& val) {
  node* new_node;

  if (first == 0) {
    new_node = new node(val, 0, 0);
    first = last = new_node;
    ++size;
  }

  else insert_before(val, first);
}

template<typename T>
inline void list::push_back(T const& val) {
  if (last == 0)
    push_front(val);
  else
    insert_after(val, last);
}

inline void list::pop_front (void) {
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

inline void list::pop_back (void) {
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

inline list::node* list::erase (node* pos) {
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

#endif // BOOST_SPIRIT_PRANA_LIST_HPP
