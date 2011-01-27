/*==============================================================================
    Copyright (c) 2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_EXCEPTION_HPP)
#define BSP_EXCEPTION_HPP

#include <sstream>

#include <boost/throw_exception.hpp>

#include <boost/spirit/home/support/info.hpp>

#include <boost/spirit/home/prana/adt/source_location.hpp>

namespace boost {
namespace spirit {
namespace prana {

// TODO: make prana exceptions boost.exception compatible
    
///////////////////////////////////////////////////////////////////////////////
struct exception {
  std::string msg;

  exception (void) {
    msg = "(unknown-exception)";
  }
 
  template<class Source> 
  exception (Source const& source, source_location loc,
             std::string const& exception)
  {
    set(source, loc, exception); 
  }

  template<class Source>
  void set (Source const& source, source_location loc,
            std::string const& exception)
  {
    msg = "(exception \"" + source + "\" " + loc.get() + " '" + exception + ")";    
  }

  std::string const& what (void) const {
    return msg;
  }
};

///////////////////////////////////////////////////////////////////////////////
struct unexpected_character_data: prana::exception {
  template<class Source>
  unexpected_character_data (Source const& source):
    prana::exception(source, make_source_location(-1, -1),
                     "(unexpected-character-data)") { }
};

///////////////////////////////////////////////////////////////////////////////
struct invalid_list_subtype: prana::exception {
  template<class Typeinfo>
  static std::string make (Typeinfo type) {
    using karma::uint_generator;
    using karma::generate;
    uint_generator<Typeinfo, 10> typeinfo_;

    typedef std::back_insert_iterator<std::string> sink_type;

    std::string output;
    sink_type sink(output);

    if (generate(sink, "(invalid-list-subtype " << typeinfo_ << ')', type))
      return output;
    else
      return "(invalid-list-subtype)"; 
  }

  template<class Source, class Typeinfo>
  invalid_list_subtype (Source const& source, source_location loc,
                        Typeinfo type):
    prana::exception(source, loc, make(type)) { }
};

///////////////////////////////////////////////////////////////////////////////
template<class Out>
struct info_printer {
  typedef utf8_string string;

  info_printer (Out& out): out(out) {}

  void element (string const& tag, string const& value, int) const {
    if (value == "")
      out << ' ' << tag;
    else
      out << " \"" << value << '"';
  }

  Out& out;
};

struct expected_component: prana::exception {
  static std::string make (info const& w) {
    std::ostringstream oss;
    oss << "(expected-component ";

    prana::info_printer<std::ostringstream> pr(oss);
    basic_info_walker<info_printer<std::ostringstream> > walker(pr, w.tag, 0);
    boost::apply_visitor(walker, w.value);

    oss << ")";

    return oss.str();
  }

  template<class Source>
  expected_component (Source const& source, source_location loc, info const& w):
    prana::exception(source, loc, make(w)) { }
};

} // prana
} // spirit
} // boost

#endif // BSP_EXCEPTION_HPP

