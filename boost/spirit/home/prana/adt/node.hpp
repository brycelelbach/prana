/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_NODE_HPP)
#define BOOST_SPIRIT_PRANA_NODE_HPP

#include <boost/noncopyable.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/mpl/if.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Data>
struct node: private boost::noncopyable {
  typedef Data        value_type;
  typedef Data&       reference;
  typedef Data const& const_reference;
  typedef Data*       pointer;
  typedef Data const* const_pointer;
  typedef std::size_t size_type;

  template<typename T>
  node (T const&, node*, node*);

  void unlink (void);

  Data val;
  node* next;
  node* prev;
};

template<typename Data>
struct node_iterator: public boost::iterator_facade<
  node_iterator<Data>, Data, boost::bidirectional_traversal_tag
> {
 public:
  typedef typename mpl::if_<
    boost::is_const<Data>,
    node<typename boost::remove_const<Data>::type> const,
    node<Data>
  >::type node_type;

  node_iterator (void);

  node_iterator (node_type*, node_type*);

  void increment (void);
  void decrement (void);

  template<typename Iterator> bool equal (Iterator const&) const;

  typename node_iterator::reference dereference (void) const;

  node_type* curr;
  node_type* prev; // only needed for sequence
};

template<typename Data>
template<typename T>
node<Data>::node (T const& val, node* next_, node* prev_):
  val(val), next(next_), prev(prev_) {
    // we leave a line here so we can breakpoint the ctor in gdb if needed
  }

template<typename Data>
void node<Data>::unlink (void) {
  // WARN: do not call unlink unless the node comes from a utree sequence
  prev->next = next;
  next->prev = prev;
}
  
template<typename Data>
node_iterator<Data>::node_iterator (void): curr(0), prev(0) {
  // we leave a line here so we can breakpoint the ctor in gdb if needed
}

template<typename Data>
node_iterator<Data>::node_iterator (node_type* curr_, node_type* prev_):
  curr(curr_), prev(prev_) {
    // we leave a line here so we can breakpoint the ctor in gdb if needed
  }

template<typename Data>
void node_iterator<Data>::increment (void) {
  if (curr != 0) { // not at end
    prev = curr;
    curr = curr->next;
  }
}

template<typename Data>
void node_iterator<Data>::decrement (void) {
  if (prev != 0) { // not at begin
    curr = prev;
    prev = prev->prev;
  }
}

template<typename Data>
template<typename Iterator>
bool node_iterator<Data>::equal (Iterator const& other) const {
  return curr == other.curr;
}

template<typename Data>
typename node_iterator<Data>::reference
node_iterator<Data>::dereference (void) const {
  return curr->val;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_NODE_HPP
