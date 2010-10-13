/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TREE_PRINT_HPP)
#define BOOST_SPIRIT_PRANA_TREE_PRINT_HPP

#include <ostream>

#include <boost/spirit/home/prana/utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct tree_print {
  typedef void result_type;

  std::ostream& out;

  tree_print (std::ostream& out): out(out) { }

  void operator() (nil) const { out << "<nil> "; }

  template<typename T>
  void operator() (T val) const { out << val << ' '; }

  void operator() (bool b) const { out << (b ? "true" : "false") << ' '; }

  void operator() (binary_range const& b) const {
    typedef binary_range::const_iterator iterator;
    
    out << "#";
    out.width(2);
    out.fill('0');
    for (iterator i = b.begin(); i != b.end(); ++i)
      // FIXME: Can has static_cast?
      out << std::hex << int((unsigned char)*i);
    out << std::dec << "# ";
  }

  void operator() (utf8_string_range const& str) const {
    typedef utf8_string_range::const_iterator iterator;

    iterator i = str.begin();

    out << '"';
    for (; i != str.end(); ++i) out << *i;
    out << "\" ";
  }

  void operator() (utf8_symbol_range const& str) const {
    typedef utf8_symbol_range::const_iterator iterator;
    iterator i = str.begin();
    for (; i != str.end(); ++i) out << *i;
  }

  template<typename Iterator>
  void operator() (iterator_range<Iterator> const& range) const {
    typedef typename iterator_range<Iterator>::const_iterator iterator;

    (*this)('(');
    for (iterator i = range.begin(); i != range.end(); ++i) visit(*i, *this);
    (*this)(')');
  }

  void operator() (any_ptr const& p) const {
    return (*this)("<pointer>");
  }

  void operator() (function_base const& pf) const {
    return (*this)("<function>");
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TREE_PRINT_HPP
