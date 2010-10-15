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

#include <boost/swap.hpp>
#include <boost/detail/iterator.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Iterator>
struct range: public boost::detail::iterator_traits<Iterator> {
 public:
  typedef typename boost::detail::iterator_traits<Iterator> iterator_traits;
 
  typedef typename iterator_traits::reference const const_reference;
  typedef typename iterator_traits::pointer const   const_pointer;
  typedef std::size_t                               size_type;
  typedef Iterator                                  iterator;
  typedef Iterator                                  const_iterator;

  void default_construct (void);

  void copy (range const&);
  void copy (iterator, iterator);
  void copy (iterator, size_type);
  template<typename Container> void copy (Container);
  
  static range make (void);
  static range make (iterator, iterator); 
  static range make (iterator, size_type);
  template<typename Container> static range make (Container);

  void swap (range&);

  void free (void);
  
  template<typename Container>
  Container get (void) const;

  iterator begin (void) const;
  iterator end (void) const;

 private:
  iterator first;
  iterator last;
};

template<typename Iterator>
inline void range<Iterator>::default_construct (void) {
  // do nothing here; first and last aren't pointers
}

template<typename Iterator>
inline void range<Iterator>::copy (range const& other) {
  // no need to call free here
  if (*this != other) {
    first = other.first;
    last = other.last;
  }
}

template<typename Iterator>
inline void range<Iterator>::copy (iterator first_, iterator last_) {
  // no need to call free here
  first = first_;
  last = last_;
}

template<typename Iterator>
inline void range<Iterator>::copy (iterator bits, size_type len) {
  // no need to call free here
  first = bits;
  last = bits + len;
}

template<typename Iterator>
template<typename Container>
inline void range<Iterator>::copy (Container c) {
  // no need to call free here
  first = c.begin();
  last = c.end();
}

template<typename Iterator>
inline range<Iterator> range<Iterator>::make (void) {
  return range(); 
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
inline range<Iterator> range<Iterator>::make (Container c) {
  range r;
  r.copy(c);
  return r;
}

template<typename Iterator>
inline void range<Iterator>::swap (range& other) {
  boost::swap(*this, other);
}

template<typename Iterator>
inline void range<Iterator>::free (void) {
  // nothing to free here!
}

template<typename Iterator>
template<typename Container>
inline Container range<Iterator>::get (void) const {
  return Container(first, last);
}

template<typename Iterator>
inline Iterator range<Iterator>::begin (void) const {
  return first;
}

template<typename Iterator>
inline Iterator range<Iterator>::end (void) const {
  return last;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_RANGE_HPP
