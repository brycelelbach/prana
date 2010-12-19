/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INPUT_ERROR_HANDLER_HPP)
#define BOOST_SPIRIT_PRANA_INPUT_ERROR_HANDLER_HPP

#include <string>
#include <sstream>

#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>

namespace boost {
namespace spirit {
namespace prana {
    
template<class Out>
struct print_info_functor {
  typedef utf8_string string;

  print_info_functor (Out& out): out(out) {}

  void element (string const& tag, string const& value, int) const {
    if (value == "")
      out << ' ' << tag;
    else
      out << " \"" << value << '"';
  }

  Out& out;
};

struct parse_exception: std::exception { };

struct expected_component: parse_exception {
  std::string msg;

  expected_component (std::string const& source, std::size_t line,
                      info const& w)
  {
    std::ostringstream oss;
    oss << "(exception \"" << source << "\" " << ((line == -1) ? -1 : line)
        << " '(expected_component ";

    print_info_functor<std::ostringstream> pr(oss);
    basic_info_walker<print_info_functor<std::ostringstream> >
      walker(pr, w.tag, 0);

    boost::apply_visitor(walker, w.value);

    oss << "))";

    msg = oss.str();
  }

  virtual ~expected_component (void) throw() { }

  virtual const char* what (void) const throw() {
    return msg.c_str();
  }
};

template<class Iterator>
struct error_handler {
  template<class, class, class, class>
  struct result {
    typedef void type;
  };

  std::string source;

  error_handler (std::string const& source_file = "<string>"):
    source_file (source_file) { }

  void operator() (Iterator first, Iterator last, Iterator err_pos,
                   info const& what) const
  {
    Iterator eol = err_pos;
    std::size_t line = get_line(err_pos);

    throw expected_component(source, line, what);
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INPUT_ERROR_HANDLER_HPP

