/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_LINE_POS_ITERATOR_HPP)
#define BOOST_SPIRIT_PRANA_ADT_LINE_POS_ITERATOR_HPP

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/range/iterator_range.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[line_pos_iterator
template<class Iterator>
class line_pos_iterator: public iterator_adaptor<
  line_pos_iterator<Iterator>, Iterator, use_default, forward_traversal_tag
> {
 public:
  line_pos_iterator (void);

  explicit line_pos_iterator (Iterator base);

  std::size_t position (void) const;

 private:
  friend class boost::iterator_core_access;

  void increment (void);

  std::size_t line;
  typename std::iterator_traits<Iterator>::value_type prev;
};
//]

template<class Iterator>
line_pos_iterator<Iterator>::line_pos_iterator (void):
  line_pos_iterator::iterator_adaptor_(), line(1), prev(0) { }

template<class Iterator>
line_pos_iterator<Iterator>::line_pos_iterator (Iterator base):
  line_pos_iterator::iterator_adaptor_(base), line(1), prev(0) { }

template<class Iterator>
std::size_t line_pos_iterator<Iterator>::position (void) const {
  return line;
}

template<class Iterator>
void line_pos_iterator<Iterator>::increment (void) {
  typename std::iterator_traits<Iterator>::reference ref = *(this->base());

  switch (ref) {
    case '\r':
      if (prev != '\n')
        ++line;
      break;
    case '\n':
      if (prev != '\r')
        ++line;
      break;
    default:
      break;
  }

  prev = ref;
  ++this->base_reference();
}

//[get_line
template<class Iterator /*< Any iterator. >*/>
inline std::size_t get_line (Iterator const& i) {
  return -1;
}

template<class Iterator /*< line_pos_iterator version. >*/>
inline std::size_t get_line (line_pos_iterator<Iterator> const& i) {
  return i.position();
}
//]

//[get_line_start
template<class Iterator /*< Applicable to any iterator. >*/>
inline Iterator get_line_start (Iterator lower_bound, Iterator current) {
  Iterator latest = lower_bound;

  for (Iterator i = lower_bound; i != current; ++i) {
    switch (*i) {
      case '\r':
      case '\n':
        latest = i;
    }
  }

  return latest;
}
//]

//[get_current_line
template<class Iterator /*< Applicable to any iterator. >*/>
inline iterator_range<Iterator>
get_current_line (Iterator lower_bound, Iterator current,
                  Iterator upper_bound) {
  Iterator first = get_line_start(lower_bound, current);
  Iterator last = get_line_start(current, upper_bound);

  if (last == current)
    last = upper_bound;

  return iterator_range<Iterator>(first, last);
}
//]

//[get_column
template<class Iterator /*< Applicable to any iterator. >*/>
inline std::size_t
get_column (Iterator lower_bound, Iterator current, int tabs = 2) {
  std::size_t column = 1;
  Iterator first = get_line_start(lower_bound, current);

  for (Iterator i = first; i != current; ++i) {
    switch (*i) {
      case '\t':
        column += tabs - (column - 1) % tabs;
        break;
      default:
        ++column;
    }
  }

  return column;
}
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_ADT_LINE_POS_ITERATOR_HPP ->*/

