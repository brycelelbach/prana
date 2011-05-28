/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_E8FAA720_3D6C_418F_9A59_F8DCA6D27232)
#define PRANA_E8FAA720_3D6C_418F_9A59_F8DCA6D27232

#include <boost/fusion/include/at_c.hpp>

#include <prana/parse/parse_tree.hpp>
#include <prana/parse/grammar/json.hpp>

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
    utree::const_iterator it = ut.begin();
    print_key(*it); ++it;
    out << ':';
    (*this)(*it);
  }

  void print_key (utree const& ut) const {
    typedef spirit::utf8_symbol_range_type::const_iterator iterator;

    spirit::utf8_symbol_range_type range
      = ut.get<spirit::utf8_symbol_range_type>();

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
    out << "<invalid>";
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

  void operator() (spirit::binary_range_type const& str) const {
    out << "<binary>";
  }

  void operator() (spirit::utf8_string_range_type const& str) const {
    typedef spirit::utf8_string_range_type::const_iterator iterator;

    iterator it = str.begin(), end = str.end();

    out << '"';

    for (; it != end; ++it)
      out << *it;

    out << '"';
  }

  void operator() (spirit::utf8_symbol_range_type const& str) const {
    typedef spirit::utf8_symbol_range_type::const_iterator iterator;

    iterator it = str.begin(), end = str.end();

    for (; it != end; ++it)
      out << *it;
  }

  template<typename Iterator>
  void operator() (boost::iterator_range<Iterator> const& range) const {
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

  void operator() (spirit::any_ptr const& p) const {
    out << "<pointer " << &p << ">";
  }

  void operator() (spirit::function_base const& pf) const {
    out << "<function " << &pf << ">";
  }
};

} // prana

#endif // PRANA_E8FAA720_3D6C_418F_9A59_F8DCA6D27232

