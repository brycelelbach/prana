/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_SCOPE_HPP)
#define BOOST_SPIRIT_PRANA_ADT_SCOPE_HPP

#include <cstddef>

#include <boost/range/iterator_range.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[scope
template<class Iterator>
class scope: public iterator_range<Iterator*> {
 public:
  typedef std::size_t size_type;

  scope (Iterator* first = 0, Iterator* last = 0,
         scope const* parent = 0);

  scope const* outer (void) const;

  size_type level (void) const;

 private:
  scope const* parent;
  size_type depth;
};
//]

template<class Iterator>
scope<Iterator>::scope (Iterator* first, Iterator* last,
                             scope const* parent_):
  iterator_range<Iterator*>(first, last), parent(parent_),
  depth(parent_ ? parent_->depth + 1 : 0) { }

template<class Iterator>
scope<Iterator> const* scope<Iterator>::outer (void) const {
  return parent;
}

template<class Iterator>
typename scope<Iterator>::size_type scope<Iterator>::level (void) const {
  return depth;
}

} /*<- prana ->*/
} /*<- spirit ->*/ 
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_ADT_SCOPE_HPP ->*/

