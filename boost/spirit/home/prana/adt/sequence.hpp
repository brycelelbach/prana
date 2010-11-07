/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_SEQUENCE_HPP)
#define BOOST_SPIRIT_PRANA_ADT_SEQUENCE_HPP

#include <algorithm>

#include <boost/spirit/home/prana/kind.hpp>
#include <boost/spirit/home/prana/tag.hpp>
#include <boost/spirit/home/prana/adt/range.hpp>
#include <boost/spirit/home/prana/adt/sequence_iterator.hpp>

namespace boost {
namespace spirit {
namespace prana {

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

  typedef range<iterator>       range_type;
  typedef range<const_iterator> const_range_type;
  
  typedef sequence_node<Data> node_type; 

  void default_construct (void);
  
  void shallow_copy (sequence const&);
  
  void deep_copy (sequence const&);
  
  void clear (void);
  
  template<typename Container> Container get (void) const;

  // TODO (wash): Insertation needs to support shallow copying!

  template<typename T, typename Iterator>
  void insert (T const&, Iterator);

  template<typename T> void push_front (T const&);
  template<typename T> void push_back (T const&);

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

  node_type* first;
  node_type* last;
};

template<typename Data>
inline void sequence<Data>::default_construct (void) {
  first = last = 0;
}

template<typename Data>
inline void sequence<Data>::shallow_copy (sequence const& other) {
  // DISCUSS (wash): Do we have to clear here? Would it be more efficient to
  // reuse the memory allocated for nodes and/or data, or would the complexity
  // of those intrinsics make such semantics more trouble than they're worth?
  clear();

  // EXPLAIN (wash): This particular sequence_node ctor allocates recursively,
  // doing the copying work for us.
  // DISCUSS (wash): Is it more efficient to use a simple for loop to allocate
  // here, instead of recursing in sequence_node's ctor? Which can be optimized
  // best by the compiler?
  first = new node_type(other.first, 0, shallow);

  for (node_type* it = first; it != 0; it = it->next) {
    if (it->next == 0) last = it;
  }
}

template<typename Data>
inline void sequence<Data>::deep_copy (sequence const& other) {
  // DISCUSS (wash): Do we have to clear here? Would it be more efficient to
  // reuse the memory allocated for nodes and/or data, or would the complexity
  // of those intrinsics make such semantics more trouble than they're worth?
  clear();

  // EXPLAIN (wash): This particular sequence_node ctor allocates recursively,
  // doing the copying work for us.
  // DISCUSS (wash): Is it more efficient to use a simple for loop to allocate
  // here, instead of recursing in sequence_node's ctor? Which can be optimized
  // best by the compiler?
  first = new node_type(other.first, 0, deep);

  for (node_type* it = first; it != 0; it = it->next) {
    if (it->next == 0) last = it;
  }
}

template<typename Data>
inline void sequence<Data>::clear (void) {
  if (first == last) {
    if (first) delete first;
    return;
  }

  node_type* p = first;
  
  while (p) {
    node_type* next = p->next;
    delete p;
    p = next;
  } 

  first = last = 0;
}

template<typename Data>
template<typename Container>
inline Container sequence<Data>::get (void) const {
  return Container(begin(), end());
}

template<typename Data>
template<typename T, typename Iterator>
inline void sequence<Data>::insert (T const& val, Iterator pos) {
  if (!pos.curr) {
    push_back(val);
    return;
  }

  node_type* new_node = new node_type(&val, pos.curr, pos.curr->prev, deep);

  if (pos.curr->prev)
    pos.curr->prev->next = new_node;
  else
    first = new_node;

  pos.curr->prev = new_node;
}

template<typename Data>
template<typename T>
inline void sequence<Data>::push_front (T const& val) {
  node_type* new_node;

  if (first == 0) {
    new_node = new node_type(&val, 0, 0, deep);
    first = last = new_node;
  }

  else {
    new_node = new node_type(&val, first, first->prev, deep);
    first->prev = new_node;
    first = new_node;
  }
}

template<typename Data>
template<typename T>
inline void sequence<Data>::push_back (T const& val) {
  if (last == 0) push_front(val);

  else {
    node_type* new_node = new node_type(&val, last->next, last, deep);
    last->next = new_node;
    last = new_node;
  }
}

template<typename Data>
inline void sequence<Data>::pop_front (void) {
  if (!first) return;

  else if (first == last) {
    delete first;
    first = last = 0;
  }

  else {
    node_type* np = first;
    first = first->next;
    first->prev = 0;
    delete np;
  }
}

template<typename Data>
inline void sequence<Data>::pop_back (void) {
  if (!first) return;

  else if (first == last) { 
    delete first;
    first = last = 0;
  }

  else {
    node_type* np = last;
    last = last->prev;
    last->next = 0;
    delete np;
  }
}

template<typename Data>
inline typename sequence<Data>::iterator
sequence<Data>::begin (void) {
  return iterator(first, 0);
}

template<typename Data>
inline typename sequence<Data>::const_iterator
sequence<Data>::begin (void) const {
  return const_iterator(first, 0);
}

template<typename Data>
inline typename sequence<Data>::iterator
sequence<Data>::end (void) {
  return iterator(0, last);
}

template<typename Data>
inline typename sequence<Data>::const_iterator
sequence<Data>::end (void) const {
  return const_iterator(0, last);
}

template<typename Data>
template<typename Iterator>
inline typename sequence<Data>::iterator sequence<Data>::erase (Iterator pos) {
  if (pos.curr == 0) return Iterator(0, last);

  else if (pos.curr == first) {
    pop_front();
    return Iterator(first, 0);
  }

  else if (pos.curr == last) {
    pop_back();
    return Iterator(0, last);
  }

  node_type* next(pos.curr->next);
  pos.curr->unlink();
  delete pos.curr;
  return Iterator(next, next->prev);
}

template<typename Data>
template<typename Container>
inline bool sequence<Data>::operator== (Container const& c) const {
  return std::equal(begin(), end(), c.begin());
}

template<typename Data>
template<typename Container>
inline bool sequence<Data>::operator!= (Container const& c) const {
  return !std::equal(begin(), end(), c.begin());
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_SEQUENCE_HPP
