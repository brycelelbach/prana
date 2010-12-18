/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INPUT_ERROR_HANDLER_HPP)
#define BOOST_SPIRIT_PRANA_INPUT_ERROR_HANDLER_HPP

#include <string>
#include <iostream>

#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Iterator>
struct error_handler {
  template<class, class, class, class>
  struct result {
    typedef void type;
  };

  std::string source_file;

  error_handler (std::string const& source_file = "<string>"):
    source_file (source_file) { }

  void operator() (Iterator first, Iterator last, Iterator err_pos,
                   info const& what) const {
    Iterator eol = err_pos;
    int line = get_line(err_pos);

    std::cerr << "(non-fatal-exception \""
              << source_file << "\" " << line
              << " '(qi-expected \"" << what << "\"))\n";
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INPUT_ERROR_HANDLER_HPP

