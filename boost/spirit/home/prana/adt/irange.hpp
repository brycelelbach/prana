/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_IRANGE_HPP)
#define BOOST_SPIRIT_PRANA_IRANGE_HPP

#include <cstddef>

namespace boost {
namespace spirit {
namespace prana {

/*=============================================================================
Lightweight implementation of boost::iterator_range as a POD, for use in utree.
=============================================================================*/

template<typename Iterator>
struct irange {
  typedef Iterator iterator;
  typedef Iterator const_iterator;
  typedef std::size_t size_type;

  void construct (iterator f, iterator l) {
    first = f;
    last = l;
  }

  void construct (iterator b, size_type len) {
    first = b;
    last = b + len;
  }
  
  template<typename Container>
  void construct (Container c) {
    first = c.begin();
    last = c.end();
  }

  template<typename Container>
  Container get (void) const {
    return Container(first, last);
  }
  
  static irange make (iterator f, iterator l) {
    irange r;
    r.construct(f, l);
    return r;
  }

  static irange make (iterator b, size_type len) {
    irange r;
    r.construct(b, len);
    return r;
  }
  
  template<typename Container>
  static irange make (Container c) {
    irange r;
    r.construct(c);
    return r;
  }

  iterator begin (void) const { return first; }
  iterator end (void) const { return last; }

  iterator first;
  iterator last;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_IRANGE_HPP
