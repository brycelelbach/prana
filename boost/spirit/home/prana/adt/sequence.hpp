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

#include <boost/spirit/home/prana/adt/sequence_fwd.hpp>
#include <boost/spirit/home/prana/adt/sequence_iterator.hpp>
#include <boost/spirit/home/prana/adt/range.hpp>
#include <boost/spirit/home/prana/utree_fwd.hpp>
#include <boost/spirit/home/prana/tag.hpp>

namespace boost {
namespace spirit {
namespace prana {

inline void sequence::default_construct (void) {
  first = last = 0;
}

inline void sequence::shallow_copy (sequence const& other) {
  // DISCUSS (wash): Do we have to clear here? Would it be more efficient to
  // reuse the memory allocated for nodes and/or data, or would the complexity
  // of those intrinsics make such semantics more trouble than they're worth?
  clear();

  // EXPLAIN (wash): This particular sequence_node ctor allocates recursively,
  // doing the copying work for us.
  // DISCUSS (wash): Is it more efficient to use a simple for loop to allocate
  // here, instead of recursing in sequence_node's ctor? Which can be optimized
  // best by the compiler?
  first = new sequence_node(other.first, 0, shallow);

  for (sequence_node* it = first; it != 0; it = it->next) {
    if (it->next == 0) last = it;
  }
}

inline void sequence::deep_copy (sequence const& other) {
  // DISCUSS (wash): Do we have to clear here? Would it be more efficient to
  // reuse the memory allocated for nodes and/or data, or would the complexity
  // of those intrinsics make such semantics more trouble than they're worth?
  clear();

  // EXPLAIN (wash): This particular sequence_node ctor allocates recursively,
  // doing the copying work for us.
  // DISCUSS (wash): Is it more efficient to use a simple for loop to allocate
  // here, instead of recursing in sequence_node's ctor? Which can be optimized
  // best by the compiler?
  first = new sequence_node(other.first, 0, deep);

  for (sequence_node* it = first; it != 0; it = it->next) {
    if (it->next == 0) last = it;
  }
}

inline void sequence::clear (void) {
  if (first == last) {
    if (first) delete first;
    return;
  }

  sequence_node* p = first;
  
  while (p) {
    sequence_node* next = p->next;
    delete p;
    p = next;
  } 

  first = last = 0;
}

template<typename Container>
inline Container sequence::get (void) const {
  return Container(begin(), end());
}

template<typename T, typename Iterator>
inline void sequence::insert (T const& val, Iterator pos) {
  if (!pos.curr) {
    push_back(val);
    return;
  }

  sequence_node* new_node = new sequence_node(val, pos.curr, pos.curr->prev);

  if (pos.curr->prev)
    pos.curr->prev->next = new_node;
  else
    first = new_node;

  pos.curr->prev = new_node;
}

template<typename T>
inline void sequence::push_front (T const& val) {
  sequence_node* new_node;

  if (first == 0) {
    new_node = new sequence_node(val, 0, 0);
    first = last = new_node;
  }

  else {
    new_node = new sequence_node(val, first, first->prev);
    first->prev = new_node;
    first = new_node;
  }
}

template<typename T>
inline void sequence::push_back (T const& val) {
  if (last == 0) push_front(val);

  else {
    sequence_node* new_node = new sequence_node(val, last->next, last);
    last->next = new_node;
    last = new_node;
  }
}

inline void sequence::pop_front (void) {
  if (!first) return;

  else if (first == last) {
    delete first;
    first = last = 0;
  }

  else {
    sequence_node* np = first;
    first = first->next;
    first->prev = 0;
    delete np;
  }
}

inline void sequence::pop_back (void) {
  if (!first) return;

  else if (first == last) { 
    delete first;
    first = last = 0;
  }

  else {
    sequence_node* np = last;
    last = last->prev;
    last->next = 0;
    delete np;
  }
}

inline sequence::iterator sequence::begin (void) {
  return iterator(first, 0);
}

inline sequence::const_iterator sequence::begin (void) const {
  return const_iterator(first, 0);
}

inline sequence::iterator sequence::end (void) {
  return iterator(0, last);
}

inline sequence::const_iterator sequence::end (void) const {
  return const_iterator(0, last);
}

template<typename Iterator>
inline sequence::iterator sequence::erase (Iterator pos) {
  if (pos.curr == 0) return Iterator(0, last);

  else if (pos.curr == first) {
    pop_front();
    return Iterator(first, 0);
  }

  else if (pos.curr == last) {
    pop_back();
    return Iterator(0, last);
  }

  sequence_node* next(pos.curr->next);
  pos.curr->unlink();
  delete pos.curr;
  return Iterator(next, next->prev);
}

template<typename Container>
inline bool sequence::operator== (Container const& c) const {
  return std::equal(begin(), end(), c.begin());
}

template<typename Container>
inline bool sequence::operator!= (Container const& c) const {
  return !std::equal(begin(), end(), c.begin());
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_SEQUENCE_HPP
