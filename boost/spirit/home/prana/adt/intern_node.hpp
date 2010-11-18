/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INTERN_NODE_HPP)
#define BOOST_SPIRIT_PRANA_INTERN_NODE_HPP

#include <boost/noncopyable.hpp>
#include <boost/detail/iterator.hpp>
#include <boost/range/iterator_range.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Iterator>
struct intern_node: private noncopyable {
  typedef Iterator id_type;
  
  typedef iterator_range<Iterator> value_type;
  typedef iterator_range<Iterator>* pointer;
  typedef iterator_range<Iterator> const* const_pointer;
  
  intern_node (id_type);

  struct destruct {
    typedef void result_type;

    template<class Alloc>
    void operator() (intern_node*, Alloc*) const;
  };

  struct find {
    typedef pointer result_type;

    template<class InputIterator>
    pointer operator() (intern_node*, InputIterator&, InputIterator) const;
    
    template<class InputIterator>
    const_pointer operator() (intern_node const*, InputIterator&,
                              InputIterator) const;
    
    template<class Pointer, class Node, class InputIterator>
    Pointer operator() (Node, InputIterator&, InputIterator) const;
  };

  struct insert {
    typedef pointer result_type;

    template<class InputIterator, class Alloc>
    pointer operator() (intern_node*&, InputIterator, InputIterator,
                        Alloc*) const;
  };

  struct erase {
    typedef pointer result_type;

    template<class InputIterator, class Alloc>
    void operator() (intern_node*&, InputIterator, InputIterator, Alloc*) const;
  };

  id_type id;        /* The node's identity character. */
  pointer data;      /* Optional data, a string. */
  intern_node* lt;   /* Left pointer. */
  intern_node* eq;   /* Middle pointer. */
  intern_node* gt;   /* Right pointer. */
};

template<class Iterator>
intern_node<Iterator>::intern_node (id_type id_):
  id(id_), data(0), lt(0), eq(0), gt(0) { }

template<class Iterator>
template<class Alloc>
inline void intern_node<Iterator>::destruct::operator() (
  intern_node* p, Alloc* alloc
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
template<class Iterator>
template<class InputIterator>
inline typename intern_node<Iterator>::pointer
intern_node<Iterator>::find::operator() (
  intern_node* start, InputIterator& first, InputIterator last
) const {
  return (*this).operator()<
    pointer, intern_node*
  >(start, first, last);
}

template<class Iterator>
template<class InputIterator>
inline typename intern_node<Iterator>::const_pointer
intern_node<Iterator>::find::operator() (
  intern_node const* start, InputIterator& first, InputIterator last
) const {
  return (*this).operator()<
    const_pointer, intern_node const*
  >(start, first, last);
}

template<class Iterator>
template<class Pointer, class Node, class InputIterator>
inline Pointer intern_node<Iterator>::find::operator() (
  Node start, InputIterator& first, InputIterator last
) const {
  if (first == last)
    return 0;

  InputIterator i = first;
  InputIterator latest = first;
  Node p = start;
  Pointer found = 0;

  while (p && i != last) {
    typename boost::detail::iterator_traits<InputIterator>::value_type c = *i; 

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
    first = ++latest; /* one past the last matching char */

  return found;
}

template<class Iterator>
template<class InputIterator, class Alloc>
inline typename intern_node<Iterator>::pointer
intern_node<Iterator>::insert::operator() (
  intern_node*& start, InputIterator first, InputIterator last, Alloc* alloc
) const {
  if (first == last)
    return 0;

  InputIterator it = first;
  intern_node** pp = &start;

  for (;;) {
    typename boost::detail::iterator_traits<InputIterator>::value_type c = *it;

    if (*pp == 0)
      *pp = alloc->new_node(it);

    intern_node* p = *pp;

    if (c == *p->id) {
      if (++it == last) {
        if (p->data == 0)
          p->data = alloc->new_data(first, last);

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

template<class Iterator>
template<class InputIterator, class Alloc>
inline void intern_node<Iterator>::erase::operator() (
  intern_node*& p, InputIterator first, InputIterator last, Alloc* alloc
) const {
  if (p == 0 || first == last)
    return;

  typename boost::detail::iterator_traits<InputIterator>::value_type c = *first;

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

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INTERN_NODE_HPP

