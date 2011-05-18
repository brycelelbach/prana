/*==============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_2C88356C_6695_4DD2_B8B4_02ADFB80CE3F)
#define PRANA_2C88356C_6695_4DD2_B8B4_02ADFB80CE3F

#include <boost/detail/iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/range/iterator_range.hpp>

#include <prana/diagnostics/source_location.hpp>

namespace prana {

template <typename Iterator, std::size_t TabLength = 2>
class position_iterator: public boost::iterator_adaptor<
  position_iterator<Iterator>, Iterator, boost::use_default,
  boost::forward_traversal_tag 
> {
 public:
  position_iterator (void):
    position_iterator::iterator_adaptor_(),
    loc(make_source_location(1, 0)), prev(0) { }

  explicit position_iterator (Iterator base):
    position_iterator::iterator_adaptor_(base),
    loc(make_source_location(1, 0)), prev(0) { }

  std::size_t& line (void) {
    return loc.line();
  }

  std::size_t const& line (void) const {
    return loc.line();
  }
   
  std::size_t& column (void) {
    return loc.column();
  }

  std::size_t const& column (void) const {
    return loc.column();
  }

  source_location& location (void) {
    return loc;
  }

  source_location const& location (void) const {
    return loc;
  }

 private:
  friend class boost::iterator_core_access;

  void increment (void) {
    using fusion::at_c;

    typename boost::detail::iterator_traits<Iterator>::reference
      ref = *(this->base());
  
    switch (ref) {
      case '\r':
        if (prev != '\n') {
          ++at_c<0>(loc);
          at_c<1>(loc) = 0;
        }
        break;
      case '\n':
        if (prev != '\r') {
          ++at_c<0>(loc);
          at_c<1>(loc) = 0;
        }
        break;
      case '\t':
        at_c<1>(loc) += TabLength;
        break;
      default:
        ++at_c<1>(loc);
        break;
    }
  
    prev = ref;
    ++this->base_reference();
  }

  source_location loc;
  typename boost::detail::iterator_traits<Iterator>::value_type prev;
};

template<class Iterator>
inline source_location get_location (Iterator i) {
  source_location loc = {-1, -1};
  return loc;
}

template<class Iterator>
inline source_location get_location (position_iterator<Iterator> i) {
  return i.location();
}

} // prana

#endif // PRANA_2C88356C_6695_4DD2_B8B4_02ADFB80CE3F
