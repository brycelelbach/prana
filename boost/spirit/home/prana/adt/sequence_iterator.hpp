/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_SEQUENCE_ITERATOR_HPP)
#define BOOST_SPIRIT_PRANA_ADT_SEQUENCE_ITERATOR_HPP

#include <boost/call_traits.hpp>
#include <boost/noncopyable.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include <boost/spirit/home/prana/utree_fwd.hpp>
#include <boost/spirit/home/prana/fn/copy.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct sequence_node: private boost::noncopyable {
  sequence_node (sequence_node*, sequence_node*, shallow_copy);
  
  sequence_node (sequence_node*, sequence_node*, deep_copy);

  template<typename T>
    sequence_node (T const&, sequence_node*, sequence_node*);

  ~sequence_node (void);

  void unlink (void);

  utree* val;
  sequence_node* next;
  sequence_node* prev;
};

sequence_node::sequence_node (
  sequence_node* other, sequence_node* prev, shallow_copy copy
):
  val(other->val),
  next((other->next ? new sequence_node(other->next, this, copy) : 0)),
  prev(prev)
{
  // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
}

sequence_node::sequence_node (
  sequence_node* other, sequence_node* prev, deep_copy copy
):
  val((other->val ? new utree(*other->val) : 0)),
  next((other->next ? new sequence_node(other->next, this, copy) : 0)),
  prev(prev)
{
  // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
}

template<typename T>
sequence_node::sequence_node (
  T const& val, sequence_node* next_, sequence_node* prev_
): val(new utree(val)), next(next_), prev(prev_) {
  // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
}

sequence_node::~sequence_node (void) {
  if (val) delete val;
}

void sequence_node::unlink (void) {
  prev->next = next;
  next->prev = prev;
}

struct sequence_iterator: public boost::iterator_facade<
  sequence_iterator, utree, boost::bidirectional_traversal_tag
> {
 public:
  sequence_iterator (void);

  sequence_iterator (sequence_node*, sequence_node*);

  void increment (void);
  void decrement (void);

  template<typename Iterator>
    bool equal (typename call_traits<Iterator>::param_type) const;

  sequence_iterator::reference dereference (void) const;

  sequence_node* curr;
  sequence_node* prev;
};

sequence_iterator::sequence_iterator (void):
  curr(0), prev(0) {
    // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
  }

sequence_iterator::sequence_iterator (
  sequence_node* curr_, sequence_node* prev_
):
  curr(curr_), prev(prev_) {
    // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
  }

inline void sequence_iterator::increment (void) {
  if (curr != 0) { 
    prev = curr;
    curr = curr->next;
  }
}

inline void sequence_iterator::decrement (void) {
  if (prev != 0) {
    curr = prev;
    prev = prev->prev;
  }
}

template<typename Iterator>
inline bool sequence_iterator::equal (
  typename call_traits<Iterator>::param_type other
) const {
  return curr == other.curr;
}

inline sequence_iterator::reference
sequence_iterator::dereference (void) const {
  return *curr->val;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_SEQUENCE_ITERATOR_HPP
