/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SUPPORT_IGNORE_UTF8_BOM_HPP)
#define BOOST_SPIRIT_PRANA_SUPPORT_IGNORE_UTF8_BOM_HPP

#include <string>
#include <fstream>
#include <exception>

namespace boost {
namespace spirit {
namespace prana {

struct io_exception: std::exception { };

struct input_error: io_exception {
  ~input_error (void) throw() { }

  virtual char const* what (void) const throw() {
    return "'(input-error)";
  }
};

struct output_error: io_exception {
  ~output_error (void) throw() { }

  virtual char const* what (void) const throw() {
    return "'(output-error)";
  }
};

struct file_not_found: io_exception {
  std::string msg;
  
  file_not_found (std::string const& fname) {
    msg = "'(file-not-found \"" + fname + "\")"; 
  }

  ~file_not_found (void) throw() { }

  virtual char const* what (void) const throw() {
    return msg.c_str();
  }
};

struct unexpected_character_data: io_exception {
  std::string msg;
  
  unexpected_character_data (std::string const& fname) {
    msg = "'(unexpected-character-data \"" + fname + "\")"; 
  }

  ~unexpected_character_data (void) throw() { }

  virtual char const* what (void) const throw() {
    return msg.c_str();
  }
};

void ignore_utf8_bom (std::ifstream& in, std::string const& fname) {
  if (!in)
    throw file_not_found(fname);

  // Ignore the BOM marking the beginning of a UTF-8 file in Windows
  char c = in.peek();

  if (c == '\xef') {
    char s[3];
    in >> s[0] >> s[1] >> s[2];
    s[3] = '\0';
    
    if (s != std::string("\xef\xbb\xbf"))
      throw unexpected_character_data(fname);
  }
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SUPPORT_IGNORE_UTF8_BOM_HPP

