/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SYMBOL_TABLE_HPP)
#define BOOST_SPIRIT_PRANA_SYMBOL_TABLE_HPP

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/container/vector/vector_fwd.hpp>
#include <boost/fusion/include/vector_fwd.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <boost/call_traits.hpp>
#include <boost/noncopyable.hpp>
#include <boost/detail/iterator.hpp>
#include <boost/range/iterator_range.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Iterator, class Data>
struct symbol_node: private noncopyable {
  typedef Iterator id_type;

  typedef iterator_range<Iterator> key_type;
  typedef Data mapped_type;
  typedef fusion::vector2<key_type, mapped_type> value_type;
  typedef value_type* pointer;
  typedef value_type const* const_pointer;
  
  symbol_node (id_type);

  struct destruct {
    typedef void result_type;

    template<class Alloc>
    void operator() (symbol_node*, Alloc*) const;
  };

  struct find {
    typedef pointer result_type;

    template<class InputIterator>
    pointer operator() (symbol_node*, InputIterator&, InputIterator) const;
    
    template<class InputIterator>
    const_pointer operator() (symbol_node const*, InputIterator&,
                              InputIterator) const;
    
    template<class Pointer, class Node, class InputIterator>
    Pointer operator() (Node, InputIterator&, InputIterator) const;
  };

  struct insert {
    typedef pointer result_type;

    template<class InputIterator, class Value, class Alloc>
    pointer operator() (symbol_node*&, InputIterator, InputIterator,
                        Value const&, Alloc*) const;
  };

  struct erase {
    typedef pointer result_type;

    template<class InputIterator, class Alloc>
    void operator() (symbol_node*&, InputIterator, InputIterator, Alloc*) const;
  };

  id_type id;        /* The node's identity character. */
  pointer data;      /* Optional data, a string. */
  symbol_node* lt;   /* Left pointer. */
  symbol_node* eq;   /* Middle pointer. */
  symbol_node* gt;   /* Right pointer. */
};

template<class Iterator, class Data>
symbol_node<Iterator, Data>::symbol_node (id_type id_):
  id(id_), data(0), lt(0), eq(0), gt(0) { }

template<class Iterator, class Data>
template<class Alloc>
inline void symbol_node<Iterator, Data>::destruct::operator() (
  symbol_node* p, Alloc* alloc
) const {
  if (p) {
    if (p->data)
      alloc->delete_data(p->data);

    (*this)(p->lt, alloc);
    (*this)(p->eq, alloc);
    (*this)(p->gt, alloc);
    alloc->delete_node(p);
  }
}
template<class Iterator, class Data>
template<class InputIterator>
inline typename symbol_node<Iterator, Data>::pointer
symbol_node<Iterator, Data>::find::operator() (
  symbol_node* root, InputIterator& it, InputIterator end
) const {
  return (*this).operator()<pointer, symbol_node*>(root, it, end);
}

template<class Iterator, class Data>
template<class InputIterator>
inline typename symbol_node<Iterator, Data>::const_pointer
symbol_node<Iterator, Data>::find::operator() (
  symbol_node const* root, InputIterator& it, InputIterator end
) const {
  return (*this).operator()<const_pointer, symbol_node const*>(root, it, end);
}

template<class Iterator, class Data>
template<class Pointer, class Node, class InputIterator>
inline Pointer symbol_node<Iterator, Data>::find::operator() (
  Node root, InputIterator& it, InputIterator end
) const {
  if (it == end)
    return 0;

  InputIterator i = it;
  InputIterator latest = it;
  Node p = root;
  Pointer found = 0;

  while (p && i != end) {
    typename detail::iterator_traits<InputIterator>::value_type c = *i; 

    if (c == *p->id) {
      if (p->data) {
        found = p->data;
        latest = i;
      }

      p = p->eq;
      i++;
    }

    else if (c < *p->id) 
      p = p->lt;

    else
      p = p->gt;
  }

  if (found)
    it = ++latest; /* one past the end matching char */

  return found;
}

template<class Iterator, class Data>
template<class InputIterator, class Value, class Alloc>
inline typename symbol_node<Iterator, Data>::pointer
symbol_node<Iterator, Data>::insert::operator() (
  symbol_node*& root, InputIterator first, InputIterator last,
  Value const& val, Alloc* alloc
) const {
  if (first == last)
    return 0;

  InputIterator it = first;
  symbol_node** pp = &root;

  for (;;) {
    typename detail::iterator_traits<InputIterator>::value_type c = *it;

    if (*pp == 0)
      *pp = alloc->new_node(it);

    symbol_node* p = *pp;

    if (c == *p->id) {
      if (++it == last) {
        if (p->data == 0)
          p->data = alloc->new_data(first, last, val);

        return p->data;
      }

      pp = &p->eq;
    }

    else if (c < *p->id)
      pp = &p->lt;

    else
      pp = &p->gt;
  }
}

template<class Iterator, class Data>
template<class InputIterator, class Alloc>
inline void symbol_node<Iterator, Data>::erase::operator() (
  symbol_node*& p, InputIterator first, InputIterator last, Alloc* alloc
) const {
  if (p == 0 || first == last)
    return;

  typename detail::iterator_traits<InputIterator>::value_type c = *first;

  if (c == *p->id) {
    if (++first == last) {
      if (p->data) {
        alloc->delete_data(p->data);
        p->data = 0;
      }
    }

    (*this)(p->eq, first, last, alloc);
  }

  else if (c < *p->id)
    (*this)(p->lt, first, last, alloc);
  
  else
    (*this)(p->gt, first, last, alloc);

  if (p->data == 0 && p->lt == 0 && p->eq == 0 && p->gt == 0) {
    alloc->delete_node(p);
    p = 0;
  }
}

template<class Iterator, class Data>
class symbol_table: private noncopyable {
 public:
  typedef Iterator id_type;
  
  typedef iterator_range<Iterator> key_type;
  typedef Data mapped_type;
  typedef fusion::vector2<key_type, mapped_type> value_type;
  typedef value_type* pointer;
  typedef value_type const* const_pointer;

  typedef symbol_node<Iterator, Data> node;

  symbol_table (void);

  ~symbol_table (void);

  template<class InputIterator>
    pointer find (InputIterator&, InputIterator);
  template<class InputIterator>
    const_pointer find (InputIterator&, InputIterator) const;

  template<class InputIterator, class Value>
    pointer insert (InputIterator, InputIterator, Value const&);

  template<class InputIterator>
    void erase (InputIterator, InputIterator);

  void clear (void);

 protected:
  friend struct symbol_node<Iterator, Data>;

  node* new_node (id_type);

  template<class InputIterator, class Value>
    pointer new_data (InputIterator, InputIterator, Value const&);

  void delete_node (node*);

  void delete_data (pointer);

 private:
  node* root;
};

template<class Iterator, class Data>
symbol_table<Iterator, Data>::symbol_table (void): root(0) { }

template<class Iterator, class Data>
symbol_table<Iterator, Data>::~symbol_table (void) {
  clear();
}

template<class Iterator, class Data>
template<class InputIterator>
typename symbol_table<Iterator, Data>::pointer
symbol_table<Iterator, Data>::find (InputIterator& first, InputIterator last) {
  return (root ? typename node::find()(root, first, last) : 0);
}

template<class Iterator, class Data>
template<class InputIterator>
typename symbol_table<Iterator, Data>::const_pointer
symbol_table<Iterator, Data>::find (InputIterator& first, InputIterator last) const {
  return (root ? typename node::find()(root, first, last) : 0);
}

template<class Iterator, class Data>
template<class InputIterator, class Value>
typename symbol_table<Iterator, Data>::pointer
symbol_table<Iterator, Data>::insert (InputIterator first, InputIterator last,
                                      Value const& val) {
  return typename node::insert()(root, first, last, val, this);
}

template<class Iterator, class Data>
template<class InputIterator>
void symbol_table<Iterator, Data>::erase (InputIterator first, InputIterator last) {
  return typename node::erase()(root, first, last, this);
}

template<class Iterator, class Data>
void symbol_table<Iterator, Data>::clear (void) {
  if (root) {
    typename node::destruct()(root, this);
    root = 0;
  }
}
template<class Iterator, class Data>
typename symbol_table<Iterator, Data>::node*
symbol_table<Iterator, Data>::new_node (id_type id) {
  return new node(id);
}

template<class Iterator, class Data>
template<class InputIterator, class Value>
typename symbol_table<Iterator, Data>::pointer
symbol_table<Iterator, Data>::new_data (InputIterator first, InputIterator last,
                                        Value const& val) {
  return new value_type(key_type(first, last), val);
}

template<class Iterator, class Data>
void symbol_table<Iterator, Data>::delete_node (node* p) {
  if (p)
    delete p;
}

template<class Iterator, class Data>
void symbol_table<Iterator, Data>::delete_data (pointer p) {
  if (p)
    delete p;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SYMBOL_TABLE_HPP

