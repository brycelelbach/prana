/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_LIST_FWD_HPP)
#define BOOST_SPIRIT_PRANA_LIST_FWD_HPP

namespace boost {
namespace spirit {
namespace prana {

// Our POD double linked list. Straightforward implementation.
// This implementation is very primitive and is not meant to be
// used stand-alone. This is the internal data representation
// of lists in our utree.

struct list {
 public:
  struct node;

  template<typename Value>
  class iterator;

  void free (void);
  void copy (list const& other);
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

  node* erase(node* pos);

  node* first;
  node* last;
  std::size_t size;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_LIST_FWD_HPP
