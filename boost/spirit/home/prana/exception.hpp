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

#include <boost/spirit/home/prana/magic.hpp>
#include <boost/spirit/home/prana/adt/source_location.hpp>

namespace boost {
namespace spirit {
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
  {
    set(source, loc, exception); 
  }

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
struct invalid_typeinfo: prana::exception {
  template<class Typeinfo>
  static std::string make (Typeinfo type, std::string const& e) {
    using karma::uint_generator;
    using karma::lit;
    using karma::generate;

    uint_generator<Typeinfo, 10> typeinfo_;

    typedef std::back_insert_iterator<std::string> sink_type;

    std::string output;
    sink_type sink(output);

    if (generate(sink, lit('(') << e << ' ' << typeinfo_ << ')', type))
      return output;
    else
      return std::string("(") + e + ")"; 
  }

  template<class Source, class Typeinfo>
  invalid_typeinfo (Source const& source, source_location loc,
                    Typeinfo type, std::string e = "invalid-typeinfo"):
    prana::exception(source, loc, make(type, e)) { }
  
  ~invalid_typeinfo (void) throw() { }
};

struct invalid_list_subtype: invalid_typeinfo {
  template<class Source, class Typeinfo>
  invalid_list_subtype (Source const& source, source_location loc,
                        Typeinfo type):
    invalid_typeinfo(source, loc, type, "invalid-list-subtype") { }

  ~invalid_list_subtype (void) throw() { }
};

struct invalid_visitable_typeinfo: invalid_typeinfo {
  template<class Typeinfo>
  invalid_visitable_typeinfo (Typeinfo type):
    invalid_typeinfo("", make_source_location(-1, -1), type,
                     "invalid-visitable-typeinfo") { }

  ~invalid_visitable_typeinfo (void) throw() { }
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
  static std::string make (info const& w, std::string const& e) {
    std::ostringstream oss;
    oss << "(" << e << " ";

    prana::info_printer<std::ostringstream> pr(oss);
    basic_info_walker<info_printer<std::ostringstream> > walker(pr, w.tag, 0);
    boost::apply_visitor(walker, w.value);

    oss << ")";

    return oss.str();
  }

  template<class Source>
  expected_component (Source const& source, source_location loc, info const& w,
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

struct expected_variant_type: expected {
  template<class Got>
  expected_variant_type (Got const& got):
    expected(got, "expected-variant-type") { }
  
  virtual ~expected_variant_type (void) throw() { }
};

struct expected_valid_pointer: expected {
  template<class Got>
  expected_valid_pointer (Got const& got):
    expected(got, "expected-valid-pointer") { }
  
  virtual ~expected_valid_pointer (void) throw() { }
};

struct expected_dynamic_array: expected {
  template<class Got>
  expected_dynamic_array (Got const& got):
    expected(got, "expected-dynamic-array") { }

  template<class Expect, class Got>
  expected_dynamic_array (Expect const& expect, Got const& got):
    expected(expect, got, "expected-dynamic-array") { }

  virtual ~expected_dynamic_array (void) throw() { }
};

struct expected_cons: expected {
  template<class Got>
  expected_cons (Got const& got):
    expected(got, "expected-cons") { }

  template<class Expect, class Got>
  expected_cons (Expect const& expect, Got const& got):
    expected(expect, got, "expected-cons") { }

  virtual ~expected_cons (void) throw() { }
};

struct expected_composite: expected {
  template<class Got>
  expected_composite (Got const& got):
    expected(got, "expected-composite") { }

  template<class Expect, class Got>
  expected_composite (Expect const& expect, Got const& got):
    expected(expect, got, "expected-composite") { }

  virtual ~expected_composite (void) throw() { }
};

struct expected_storage: expected {
  template<class Got>
  expected_storage (Got const& got):
    expected(got, "expected-storage") { }

  template<class Expect, class Got>
  expected_storage (Expect const& expect, Got const& got):
    expected(expect, got, "expected-storage") { }

  virtual ~expected_storage (void) throw() { }
};

} // prana
} // spirit
} // boost

#endif // BSP_EXCEPTION_HPP

