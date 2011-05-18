/*==============================================================================
    Copyright (c) 2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_985A446E_54F8_42BC_9E61_9AF1FC2D0EC1)
#define PRANA_985A446E_54F8_42BC_9E61_9AF1FC2D0EC1

#include <sstream>
#include <exception>

#include <boost/spirit/home/support/info.hpp>

#include <prana/magic.hpp>
#include <prana/diagnostics/source_location.hpp>

namespace prana {

///////////////////////////////////////////////////////////////////////////////
struct exception: std::exception {
  std::string msg;

  exception (void) {
    msg = "(unknown-exception)";
  }
 
  template<class Source> 
  exception (Source const& source, source_location loc,
             std::string const& exception)
  { set(source, loc, exception); }

  template<class Source>
  void set (Source const& source, source_location loc,
            std::string const& exception)
  {
    msg = "(exception (";

    typename magic::get_string_from_type<Source const>::type source_ =
      magic::stringify(source);

    if (!source_.empty())
      msg += "\"" + source_ + "\"";

    if (valid_location(loc))
      msg += " " + loc.get();

    msg += ") '" + exception + ")";
  }

  const char* what (void) const throw() {
    return msg.c_str();
  }

  ~exception (void) throw() { }
};

///////////////////////////////////////////////////////////////////////////////
struct unexpected_character_data: prana::exception {
  template<class Source>
  unexpected_character_data (Source const& source,
                             std::string e = "unexpected-character-data"):
    prana::exception(source, make_source_location(-1, -1),
                     std::string("(") + e + ")") { }
  
  ~unexpected_character_data (void) throw() { }
};

///////////////////////////////////////////////////////////////////////////////
template<class Out>
struct info_printer {
  typedef spirit::utf8_string string;

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
  static std::string make (spirit::info const& w, std::string const& e) {
    std::ostringstream oss;
    oss << "(" << e << " ";

    prana::info_printer<std::ostringstream> pr(oss);
    spirit::basic_info_walker<
      info_printer<std::ostringstream>
    > walker(pr, w.tag, 0);
    boost::apply_visitor(walker, w.value);

    oss << ")";

    return oss.str();
  }

  template<class Source>
  expected_component (Source const& source, source_location loc,
                      spirit::info const& w,
                      std::string e = "expected-component"):
    prana::exception(source, loc, make(w, e)) { }
  
  ~expected_component (void) throw() { }
};

///////////////////////////////////////////////////////////////////////////////
struct expected: prana::exception {
  template<class Got>
  static std::string make (Got const& got, std::string const& e) {
    std::ostringstream oss;
    oss << "(" << e << " " << got << ")"; 
    return oss.str();
  }

  template<class Expect, class Got>
  static std::string make (Expect const& expect, Got const& got,
                           std::string const& e)
  {
    std::ostringstream oss;
    oss << "(" << e << " " << expect << " " << got << ")"; 
    return oss.str();
  }
  
  template<class Got>
  expected (std::string e):
    prana::exception("", make_source_location(-1, -1),
                     std::string("(") + e + ")") { }

  template<class Got>
  expected (Got const& got, std::string e):
    prana::exception("", make_source_location(-1, -1), make(got, e)) { }

  template<class Expect, class Got>
  expected (Expect const& expect, Got const& got, std::string e):
    prana::exception("", make_source_location(-1, -1), make(expect, got, e)) { }

  virtual ~expected (void) throw() { }
};

} // prana

#endif // PRANA_985A446E_54F8_42BC_9E61_9AF1FC2D0EC1

