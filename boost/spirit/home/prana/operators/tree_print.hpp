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
  typedef std::ostream& result_type;

  std::ostream& out;

  tree_print (std::ostream& out): out(out) { }

  result_type operator() (nil) const {
    out << "<nil> ";
    return out;
  }

  template<typename T>
  result_type operator() (T val) const {
    out << val << ' ';
    return out;
  }

  result_type operator() (bool b) const {
    out << (b ? "true" : "false") << ' ';
    return out;
  }

  result_type operator() (binary_range const& b) const {
    typedef binary_range::const_iterator iterator;
    
    out << "#";
    out.width(2);
    out.fill('0');
    for (iterator i = b.begin(); i != b.end(); ++i)
      // FIXME: Can has static_cast?
      out << std::hex << int((unsigned char)*i);
    out << std::dec << "# ";

    return out;
  }

  result_type operator() (utf8_string_range const& str) const {
    typedef utf8_string_range::const_iterator iterator;

    iterator i = str.begin();

    out << '"';
    for (; i != str.end(); ++i) out << *i;
    out << "\" ";
    
    return out;
  }

  result_type operator() (utf8_symbol_range const& str) const {
    typedef utf8_symbol_range::const_iterator iterator;
    iterator i = str.begin();
    for (; i != str.end(); ++i) out << *i;
    return out;
  }

  result_type operator() (irange<char const*> const& str) const {
    char const* i = str.begin();
    for (; i != str.end(); ++i) out << *i;
    return out;
  }
 
  template<typename Iterator>
  result_type operator() (irange<Iterator> const& range) const {
    typedef typename irange<Iterator>::const_iterator iterator;

    out << "(";
    for (iterator i = range.begin(); i != range.end(); ++i) visit(*i, *this);
    out << ")";
    
    return out;
  }

  result_type operator() (any_ptr const& p) const {
    out << "<pointer>";
    return out;
  }

  result_type operator() (record<utree> const& pf) const {
    out << "<record>";
    return out;
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TREE_PRINT_HPP
