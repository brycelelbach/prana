/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_RANGE_HPP)
#define BOOST_SPIRIT_PRANA_ADT_RANGE_HPP

#include <cstddef>

#include <algorithm>

namespace boost { 
namespace spirit {
namespace prana {

template<typename Iterator>
struct range {
 public:
  typedef Iterator        value_type;
  typedef Iterator&       reference;
  typedef Iterator const& const_reference;
  typedef Iterator*       pointer;
  typedef Iterator const* const_pointer;
  typedef std::size_t     size_type;
  typedef Iterator        iterator;
  typedef Iterator        const_iterator;

  void default_construct (void);

  void shallow_copy (iterator*, iterator*);
  void shallow_copy (range&);
  
  void deep_copy (iterator, iterator);
  void deep_copy (iterator, size_type);
  template<typename Container> void deep_copy (Container const&);

  void clear (void);
  
  template<typename Container>
  Container get (void) const;

  iterator begin (void) const;
  iterator end (void) const;

  template<typename Container> bool operator== (Container const&) const;
  template<typename Container> bool operator!= (Container const&) const;

  iterator* last_ptr (void) const;

  bool alias (void) const;
  void alias (bool);

  iterator* first;
  iterator* last;
};

template<typename Iterator>
inline void range<Iterator>::default_construct (void) {
  first = last = 0;
}

template<typename Iterator>
inline void range<Iterator>::shallow_copy (iterator* first_, iterator* last_) {
  clear();
  first = first_;
  last = last_;
  alias(true);
}

template<typename Iterator>
inline void range<Iterator>::shallow_copy (range& other) {
  shallow_copy(other.first, other.last);
}

template<typename Iterator>
inline void range<Iterator>::deep_copy (iterator first_, iterator last_) {
  if (!first || alias()) first = new iterator(first_);
  else *first = first_;

  if (!last || alias()) last = new iterator(last_);
  else *last = last_;
}

template<typename Iterator>
inline void range<Iterator>::deep_copy (iterator bits, size_type len) {
  deep_copy(bits, bits + len);
}

template<typename Iterator>
template<typename Container>
inline void range<Iterator>::deep_copy (Container const& c) {
  deep_copy(c.begin(), c.end());
}

template<typename Iterator>
inline void range<Iterator>::clear (void) {
  if (first && !alias()) {
    delete first;
    first = 0;
  }
  if (last && !alias()) {
    delete last_ptr();
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
  // DISCUSS (wash): This will seg fault if the last pointer is 0 and the range
  // is an alias. Is there any reason why we would want an alias to a range with
  // a NULL last pointer? I have been unable to think of one, so I'm leaving the
  // condition as "last", instead of the more expensive "last_ptr()". A safer
  // version would be "(last_ptr() ? *last_ptr() : iterator())".
  return (last ? *last_ptr() : iterator());
}

template<typename Iterator>
template<typename Container>
inline bool range<Iterator>::operator== (Container const& c) const {
  // DISCUSS (wash): Is this too slow? Can we do this faster?
  return std::equal(begin(), end(), c.begin());
}

template<typename Iterator>
template<typename Container>
inline bool range<Iterator>::operator!= (Container const& c) const {
  // DISCUSS (wash): Is this too slow? Can we do this faster?
  return !std::equal(begin(), end(), c.begin());
}

template<typename Iterator>
inline typename range<Iterator>::iterator*
range<Iterator>::last_ptr (void) const {
  if (alias())
    return (iterator*) (((size_type) last) - 1);

  return last;
}

template<typename Iterator>
inline bool range<Iterator>::alias (void) const {
  return ((size_type) last) & 1;
}

template<typename Iterator>
inline void range<Iterator>::alias (bool alias_) {
  if (alias_ && !alias())
    last = (iterator*) (((size_type) last) + 1);
  else if (!alias_ && alias())
    last = (iterator*) (((size_type) last) - 1);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_RANGE_HPP
