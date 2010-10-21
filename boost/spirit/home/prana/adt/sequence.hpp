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

#include <boost/swap.hpp>

#include <boost/spirit/home/prana/adt/range.hpp>
#include <boost/spirit/home/prana/adt/sequence_iterator.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace adt {

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
  
  void copy (sequence const& other);
  
  static sequence make (void);
  
  void free (void);

  void swap (sequence&);
  
  template<typename Container> Container get (void) const;

  template<typename T, typename Iterator>
  void insert (T const& val, Iterator pos);

  template<typename T> void push_front (T const& val);
  template<typename T> void push_back (T const& val);

  void pop_front (void);
  void pop_back (void);

  iterator begin (void);
  const_iterator begin (void) const;

  iterator end (void);
  const_iterator end (void) const;

  template<typename Iterator>
  iterator erase (Iterator pos);

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
inline void sequence<Data>::copy (sequence const& other) {
  free();
  node_type* p = other.first;

  while (p != 0) {
    push_back(p->val);
    p = p->next;
  }
}

template<typename Data>
inline sequence<Data> sequence<Data>::make (void) {
  sequence s;
  s.default_construct();
  return s;
}

template<typename Data>
inline void sequence<Data>::swap (sequence& other) {
  boost::swap(*this, other);
}

template<typename Data>
inline void sequence<Data>::free (void) {
  if (first == last) { // we only have one element
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

  node_type* new_node = new node_type(val, pos.curr, pos.curr->prev);

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
    new_node = new node_type(val, 0, 0);
    first = last = new_node;
  }

  else {
    new_node = new node_type(val, first, first->prev);
    first->prev = new_node;
    first = new_node;
  }
}

template<typename Data>
template<typename T>
inline void sequence<Data>::push_back (T const& val) {
  if (last == 0) push_front(val);

  else {
    node_type* new_node = new node_type(val, last->next, last);
    last->next = new_node;
    last = new_node;
  }
}

template<typename Data>
inline void sequence<Data>::pop_front (void) {
  if (!first) return;

  else if (first == last) { // there's only one item
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

  else if (first == last) { // there's only one item
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

} // adt
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_SEQUENCE_HPP
