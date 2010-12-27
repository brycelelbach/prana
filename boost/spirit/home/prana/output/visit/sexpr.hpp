/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_OUTPUT_VISIT_SEXPR_HPP)
#define BOOST_SPIRIT_PRANA_OUTPUT_VISIT_SEXPR_HPP

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Out>
struct sexpr_printer {
  typedef void result_type;

  Out& out;

  sexpr_printer (Out& out): out(out) { }

  void operator() (utree::uninitialized_type) const {
    out << "#uninitialized";
  }

  void operator() (utree::nil_type) const {
    out << "nil";
  }

  template<class T>
  void operator() (T val) const {
    out << val;
  }

  void operator() (bool b) const {
    out << (b ? "#t" : "#f");
  }

  void operator() (binary_range_type const& str) const {
    typedef binary_range_type::const_iterator iterator;

    iterator it = str.begin(), end = str.end();

    out << '#';

    for (; it != end; ++it)
      out << *it;

    out << '#';
  }

  void operator() (utf8_string_range_type const& str) const {
    typedef utf8_string_range_type::const_iterator iterator;

    iterator it = str.begin(), end = str.end();

    out << '"';

    for (; it != end; ++it)
      out << *it;

    out << '"';
  }

  void operator() (utf8_symbol_range_type const& str) const {
    typedef utf8_symbol_range_type::const_iterator iterator;

    iterator it = str.begin(), end = str.end();

    for (; it != end; ++it)
      out << *it;
  }

  template<typename Iterator>
  void operator() (iterator_range<Iterator> const& range) const {
    typedef typename iterator_range<Iterator>::const_iterator iterator;

    iterator it = range.begin(), end = range.end();

    (*this)('(');

    utree::visit(*it, *this);

    for (; it != end; ++it) {
      out << ' ';
      utree::visit(*it, *this);
    }
    
    (*this)(')');
  }

  void operator() (any_ptr const& p) const {
    return (*this)("#pointer");
  }

  void operator() (function_base const& pf) const {
    return (*this)("#function");
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_OUTPUT_VISIT_SEXPR_HPP

