/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <sheol/lightweight_test.hpp>

#include <prana/parse/parse_tree.hpp>
#include <prana/parse/grammar/sexpr.hpp>
#include <prana/utree/io.hpp>

int main (void) { 
  using prana::utree;
  using prana::parse_tree;
  using prana::tag::sexpr;

  { 
    const std::string in = "nil";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "nil");
  }

  { 
    const std::string in = "#t";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "#t");
  }

  { 
    const std::string in = "10";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "10");
  }

  { 
    const std::string in = "#x10";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "16");
  }

  { 
    const std::string in = "#o10";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "8");
  }

  { 
    const std::string in = "17.5";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "17.5");
  }

  { 
    const std::string in = "\"foo\"";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "\"foo\"");
  }

  { 
    const std::string in = "\"\"";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "\"\"");
  }

  { 
    const std::string in = "(\"\\\"\" \"a\\\"\" \"\\\"b\" \"a\\\"b\")";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "(\"\"\" \"a\"\" \"\"b\" \"a\"b\")");
  }

  { 
    const std::string in = "#\xDE\xAD\xBE\xEF#";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "#\xDE\xAD\xBE\xEF#");
  }

  { 
    const std::string in = "##";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "##");
  }

  { 
    const std::string in = "(### #\xBE## ##\xEF# #\xBE#\xEF#)";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "(### #\xBE## ##\xEF# #\xBE#\xEF#)"); 
  }

  { 
    const std::string in = "bar";
    parse_tree<sexpr> pt(in);
    SHEOL_TEST_STREQ(pt.ast(), "bar");
  }
  
  return sheol::report_errors();
}
 
