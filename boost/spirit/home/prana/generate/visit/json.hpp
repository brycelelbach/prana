/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_OUTPUT_VISIT_JSON_HPP)
#define BSP_OUTPUT_VISIT_JSON_HPP

#include <boost/fusion/include/at_c.hpp>

#include <boost/spirit/home/prana/parse/parse_tree.hpp>
#include <boost/spirit/home/prana/parse/grammar/json.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Out>
struct json_printer {
  typedef void result_type;
  typedef parse_tree<tag::json>::annotations_type annotations_type;

  Out& out;
  annotations_type annotations;

  json_printer (Out& out_, annotations_type const& annotations_):
    out(out_), annotations(annotations_) { }

  void print_object (utree const& ut) const {
    typedef utree::const_iterator iterator;

    iterator it = ut.front().begin(), end = ut.front().end();

    (*this)('{');

    (*this)(*it); ++it;

    for (; it != end; ++it) {
      out << ", ";
      (*this)(*it);
    }
    
    (*this)('}');
  }

  void print_member_pair (utree const& ut) const {
    print_key(ut[0]);
    out << ':';
    (*this)(ut[1]);
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

  void print_array (utree const& ut) const {
    typedef utree::const_iterator iterator;

    iterator it = ut.begin(), end = ut.end();

    (*this)('[');

    (*this)(*it); ++it;

    for (; it != end; ++it) {
      out << ", ";
      (*this)(*it);
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
    BOOST_ASSERT(false);
  }

  void operator() (utree const& ut) const {
    using boost::fusion::at_c;

    switch (ut.which()) {
      case utree_type::reference_type:
        return (*this)(ut.deref());

      case utree_type::range_type:
      case utree_type::list_type:
        if (at_c<1>(annotations[ut.tag()]) == json_object::value) {
          print_object(ut);
          return;
        }
        else if (at_c<1>(annotations[ut.tag()]) == json_array::value) {
          print_array(ut);
          return;
        }
        else if (at_c<1>(annotations[ut.tag()]) == json_member_pair::value) {
          print_member_pair(ut);
          return;
        }

      default:
        break;
    }

    utree::visit(ut, *this);
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

#endif // BSP_OUTPUT_VISIT_JSON_HPP

