/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_OUTPUT_VISIT_JSON_HPP)
#define BOOST_SPIRIT_PRANA_OUTPUT_VISIT_JSON_HPP

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Out>
struct json_printer {
  typedef void result_type;

  Out& out;

  json_printer (Out& out): out(out) { }

  template<typename Iterator>
  static bool is_object (iterator_range<Iterator> const& range) {
    typedef typename iterator_range<Iterator>::const_iterator iterator;

    iterator it = range.begin(), end = range.end();

    for (; it != end; ++it) {
      try {
        if (it->size() != 2)
          return false;
        if (it->begin()->which() != utree_type::symbol_type)
          return false;
      }

      catch (...) {
        return false;
      }
    }

    return true;
  }

  template<typename Iterator>
  void print_object (iterator_range<Iterator> const& range) const {
    typedef typename iterator_range<Iterator>::const_iterator iterator;

    iterator it = range.begin(), end = range.end();

    (*this)('{');

    print_member_pair(*it); ++it;

    for (; it != end; ++it) {
      out << ", ";
      print_member_pair(*it);
    }
    
    (*this)('}');
  }

  void print_member_pair (utree const& ut) const {
    print_key(ut[0]);
    out << ':';
    utree::visit(ut[1], *this);
  }

  void print_key (utree const& ut) const {
    typedef utf8_symbol_range_type::const_iterator iterator;

    utf8_symbol_range_type range = ut.get<utf8_symbol_range_type>();

    iterator it = range.begin(), end = range.end();
    
    out << '"';

    for (; it != end; ++it)
      out << *it;

    out << '"';
  } 

  template<typename Iterator>
  void print_array (iterator_range<Iterator> const& range) const {
    typedef typename iterator_range<Iterator>::const_iterator iterator;

    iterator it = range.begin(), end = range.end();

    (*this)('[');

    utree::visit(*it, *this); ++it;

    for (; it != end; ++it) {
      out << ", ";
      utree::visit(*it, *this);
    }
    
    (*this)(']');
  }

  void operator() (utree::invalid_type) const {
    out << "\"invalid\"";
  }

  void operator() (utree::nil_type) const {
    out << "null";
  }

  template<class T>
  void operator() (T val) const {
    out << val;
  }

  void operator() (bool b) const {
    out << (b ? "true" : "false");
  }

  void operator() (binary_range_type const& str) const {
    out << "\"binary\"";
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
    if (is_object(range))
      print_object(range);
    else
      print_array(range);
  }

  void operator() (any_ptr const& p) const {
    return (*this)("\"pointer\"");
  }

  void operator() (function_base const& pf) const {
    return (*this)("\"function\"");
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_OUTPUT_VISIT_JSON_HPP

