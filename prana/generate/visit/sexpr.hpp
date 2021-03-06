/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_41143C6D_6CE0_4314_B918_B12618A675D6)
#define PRANA_41143C6D_6CE0_4314_B918_B12618A675D6

#include <prana/config.hpp>

#include <ios>

#include <boost/io/ios_state.hpp>

#include <prana/include/utree.hpp>

namespace prana {

template<class Out>
struct sexpr_printer {
  typedef void result_type;

  Out& out;
  const bool print_pointers;

  sexpr_printer (Out& out_, bool print_pointers_ = true):
    out(out_), print_pointers(print_pointers_) { }

  void operator() (utree const& ut) const {
    utree::visit(ut, *this);
  } 

  void operator() (utree::invalid_type) const {
    out << "#<unspecified>";
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

  void operator() (spirit::binary_range_type const& str) const {
    typedef spirit::binary_range_type::const_iterator iterator;

    iterator it = str.begin(), end = str.end();
        
    boost::io::ios_all_saver saver(out); 

    out << '#' << std::hex;

    out.width(2); 
    out.fill('0');

    for (; it != end; ++it)
      out << int((unsigned char)(*it));

    out << '#';
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
    typedef typename boost::iterator_range<Iterator>::const_iterator iterator;

    iterator it = range.begin(), end = range.end();

    (*this)('(');

    if (it != end) {
      utree::visit(*it, *this);
      ++it;
    }

    for (; it != end; ++it) {
      out << ' ';
      utree::visit(*it, *this);
    }
    
    (*this)(')');
  }

  void operator() (spirit::any_ptr const& p) const {
    if (print_pointers)
      out << "#<object " << &p << ">";
    else
      out << "#<object>";
  }

  void operator() (function_base const& pf) const {
    if (print_pointers)
      out << "#<procedure " << &pf << ">";
    else
      out << "#<procedure>";
  }
};

} // prana

#endif // PRANA_41143C6D_6CE0_4314_B918_B12618A675D6

