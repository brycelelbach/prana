/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_A5FB62EB_2B71_4A98_891B_776F9A437DFF)
#define PRANA_A5FB62EB_2B71_4A98_891B_776F9A437DFF

#include <boost/foreach.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <prana/parse/parse_tree.hpp>
#include <prana/parse/grammar/xml.hpp>

namespace prana {

template<class Out>
struct xml_printer {
  typedef void result_type;
  typedef parse_tree<tag::xml>::annotations_type annotations_type;

  Out& out;
  annotations_type annotations;

  xml_printer (Out& out_, annotations_type const& annotations_):
    out(out_), annotations(annotations_) { }

  void print_element (utree const& ut) const {
    typedef utree::const_iterator iterator;

    iterator it = ut.begin(), end = ut.end();

    out << "<";

    utree::visit(*it, *this); ++it;

    print_attributes(*it); ++it;

    out << ">";

    print_children(*it);

    out << "</";

    utree::visit(ut.front(), *this);

    out << ">";
  }
  
  void print_empty_element (utree const& ut) const {
    out << "<";

    utree::visit(ut.front(), *this);

    print_attributes(ut.back());

    out << " />";
  }

  void print_attributes (utree const& ut) const {
    BOOST_FOREACH(utree const& attribute, ut)
    { print_attribute(attribute); }    
  }

  void print_attribute (utree const& ut) const {
    out << " ";

    utree::visit(ut.front(), *this);

    out << "=\"";

    utree::visit(ut.back(), *this);

    out << "\"";
  }
  
  void print_children (utree const& ut) const {
    BOOST_FOREACH(utree const& child, ut)
    { (*this)(child); }    
  }

  void print_instruction (utree const& ut) const {
    out << "<?";

    utree::visit(ut.front(), *this);

    if (ut.back() != utree(""))
      out << " ";

    utree::visit(ut.back(), *this);

    out << " ?>"; 
  }

  void operator() (utree::invalid_type) const {
    out << "<invalid />";
  }

  void operator() (utree::nil_type) const {
    out << "<nil />";
  }

  template<class T>
  void operator() (T val) const {
    out << val;
  }

  void operator() (bool b) const {
    out << (b ? "true" : "false");
  }

  void operator() (spirit::binary_range_type const& str) const {
    out << "<binary />";
  }

  void operator() (spirit::utf8_string_range_type const& str) const {
    typedef spirit::utf8_string_range_type::const_iterator iterator;

    iterator it = str.begin(), end = str.end();

    for (; it != end; ++it)
      out << *it;
  }

  void operator() (spirit::utf8_symbol_range_type const& str) const {
    typedef spirit::utf8_symbol_range_type::const_iterator iterator;

    iterator it = str.begin(), end = str.end();

    for (; it != end; ++it)
      out << *it;
  }

  template<typename Iterator>
  void operator() (boost::iterator_range<Iterator> const& range) const {
    BOOST_FOREACH(utree const& element, range)
    { (*this)(element); }
  }

  void operator() (utree const& ut) const {
    using boost::fusion::at_c;

    switch (ut.which()) {
      case utree_type::reference_type:
        return (*this)(ut.deref());

      case utree_type::range_type:
      case utree_type::list_type:
        if (at_c<1>(annotations[ut.tag()]) == xml_element::value) {
          print_element(ut);
          return;
        }
        else if (at_c<1>(annotations[ut.tag()]) == xml_empty_element::value) {
          print_empty_element(ut);
          return;
        }
        else if (at_c<1>(annotations[ut.tag()]) == xml_instruction::value) {
          print_instruction(ut);
          return;
        }

      default:
        break;
    }

    utree::visit(ut, *this);
  }

  void operator() (spirit::any_ptr const& p) const {
    out << "<pointer value=\"" << &p << "\" />";
  }

  void operator() (spirit::function_base const& pf) const {
    out << "<function value=\"" << &pf << "\" />";
  }
};

} // prana

#endif // PRANA_A5FB62EB_2B71_4A98_891B_776F9A437DFF

