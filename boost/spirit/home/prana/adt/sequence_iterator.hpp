/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_SEQUENCE_ITERATOR_HPP)
#define BOOST_SPIRIT_PRANA_ADT_SEQUENCE_ITERATOR_HPP

#include <boost/noncopyable.hpp>

#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <boost/mpl/if.hpp>

#include <boost/iterator/iterator_facade.hpp>

#include <boost/spirit/home/prana/fn/copy.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace adt {

template<typename Data>
struct sequence_node: private boost::noncopyable {
  typedef Data        value_type;
  typedef Data&       reference;
  typedef Data const& const_reference;
  typedef Data*       pointer;
  typedef Data const* const_pointer;
  typedef std::size_t size_type;

  sequence_node (sequence_node*, sequence_node*, fn::shallow_copy);
  
  sequence_node (sequence_node*, sequence_node*, fn::deep_copy);

  template<typename T>
  sequence_node (T const&, sequence_node*, sequence_node*);

  ~sequence_node (void);

  void unlink (void);

  Data* val;
  sequence_node* next;
  sequence_node* prev;
};

template<typename Data>
sequence_node<Data>::sequence_node (
  sequence_node* other, sequence_node* prev, fn::shallow_copy copy
):
  val(other->val),
  next((other->next ? new sequence_node(other->next, this, copy) : 0)),
  prev(prev)
{
  // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
}

template<typename Data>
sequence_node<Data>::sequence_node (
  sequence_node* other, sequence_node* prev, fn::deep_copy copy
):
  val((other->val ? new Data(*other->val) : 0)),
  next((other->next ? new sequence_node(other->next, this, copy) : 0)),
  prev(prev)
{
  // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
}

template<typename Data>
template<typename T>
sequence_node<Data>::sequence_node (
  T const& val, sequence_node* next_, sequence_node* prev_
): val(new Data(val)), next(next_), prev(prev_) {
  // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
}

template<typename Data>
sequence_node<Data>::~sequence_node (void) {
  if (val) delete val;
}

template<typename Data>
void sequence_node<Data>::unlink (void) {
  prev->next = next;
  next->prev = prev;
}

template<typename Data>
struct sequence_iterator: public boost::iterator_facade<
  sequence_iterator<Data>, Data, boost::bidirectional_traversal_tag
> {
 public:
  typedef typename mpl::if_<
    boost::is_const<Data>,
    sequence_node<typename boost::remove_const<Data>::type> const,
    sequence_node<Data>
  >::type node_type;

  sequence_iterator (void);

  sequence_iterator (node_type*, node_type*);

  void increment (void);
  void decrement (void);

  template<typename Iterator> bool equal (Iterator const&) const;

  typename sequence_iterator::reference dereference (void) const;

  node_type* curr;
  node_type* prev;
};

template<typename Data>
sequence_iterator<Data>::sequence_iterator (void):
  curr(0), prev(0) {
    // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
  }

template<typename Data>
sequence_iterator<Data>::sequence_iterator (node_type* curr_, node_type* prev_):
  curr(curr_), prev(prev_) {
    // EXPLAIN (wash): We leave a line here so we can breakpoint the ctor 
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
bool sequence_iterator<Data>::equal (Iterator const& other) const {
  return curr == other.curr;
}

template<typename Data>
typename sequence_iterator<Data>::reference
sequence_iterator<Data>::dereference (void) const {
  return *curr->val;
}

} // adt
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_SEQUENCE_ITERATOR_HPP
