/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_RANGE_HPP)
#define BOOST_SPIRIT_PRANA_RANGE_HPP

#include <cstddef>

#include <algorithm>

#include <boost/detail/iterator.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Iterator>
struct range {
 public:
  typedef typename boost::detail::iterator_traits<Iterator> iterator_traits;

  typedef typename iterator_traits::value_type      value_type; 
  typedef typename iterator_traits::reference       reference; 
  typedef typename iterator_traits::reference const const_reference;
  typedef typename iterator_traits::pointer         pointer; 
  typedef typename iterator_traits::pointer const   const_pointer;
  typedef std::size_t                               size_type;
  typedef Iterator                                  iterator;
  typedef Iterator                                  const_iterator;

  void default_construct (void);

  // FIXME: add shallow copies, maybe
  void copy (iterator, iterator);
  void copy (iterator, size_type);
  template<typename Container> void copy (Container const&);
  
  static range make (void);
  static range make (iterator, iterator); 
  static range make (iterator, size_type);
  template<typename Container> static range make (Container const&);

  void free (void);
  
  template<typename Container>
  Container get (void) const;

  iterator begin (void) const;
  iterator end (void) const;
  
  template<typename Container> bool operator== (Container const&) const;
  template<typename Container> bool operator!= (Container const&) const;

  iterator* first;
  iterator* last;
};

template<typename Iterator>
inline void range<Iterator>::default_construct (void) {
  first = last = 0;
}

template<typename Iterator>
inline void range<Iterator>::copy (iterator first_, iterator last_) {
  if (!first) first = new iterator(first_);
  else *first = first_;

  if (!last) last = new iterator(last_);
  else *last = last_;
}

template<typename Iterator>
inline void range<Iterator>::copy (iterator bits, size_type len) {
  if (!first) first = new iterator(bits);
  else *first = bits;

  if (!last) last = new iterator(bits + len);
  else *last = bits + len;
}

template<typename Iterator>
template<typename Container>
inline void range<Iterator>::copy (Container const& c) {
  if (!first) first = new iterator(c.begin());
  else *first = c.begin();

  if (!last) last = new iterator(c.end());
  else *last = c.end();
}

template<typename Iterator>
inline range<Iterator> range<Iterator>::make (void) {
  range r;
  r.default_construct();
  return r; 
}

template<typename Iterator>
inline range<Iterator> range<Iterator>::make (iterator f, iterator l) {
  range r;
  r.copy(f, l);
  return r;
}

template<typename Iterator>
inline range<Iterator> range<Iterator>::make (iterator b, size_type len) {
  range r;
  r.copy(b, len);
  return r;
}

template<typename Iterator>
template<typename Container>
inline range<Iterator> range<Iterator>::make (Container const& c) {
  range r;
  r.copy(c);
  return r;
}

template<typename Iterator>
inline void range<Iterator>::free (void) {
  if (first) {
    delete first;
    first = 0;
  }
  if (last) {
    delete last;
    last = 0;
  }
}

template<typename Iterator>
template<typename Container>
inline Container range<Iterator>::get (void) const {
  return Container(begin(), end());
}

template<typename Iterator>
inline Iterator range<Iterator>::begin (void) const {
  return (first ? *first : iterator());
}

template<typename Iterator>
inline Iterator range<Iterator>::end (void) const {
  return (last ? *last : iterator());
}

template<typename Iterator>
template<typename Container>
inline bool range<Iterator>::operator== (Container const& c) const {
  return std::equal(begin(), end(), c.begin());
}

template<typename Iterator>
template<typename Container>
inline bool range<Iterator>::operator!= (Container const& c) const {
  return !std::equal(begin(), end(), c.begin());
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_RANGE_HPP
