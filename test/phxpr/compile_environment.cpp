////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <sheol/lightweight_test.hpp>

#include <phxpr/environment.hpp>

using phxpr::compile_environment;

struct item {
  std::string name;

  item (std::string const& name_ = "#<anonymous>"): name(name_)
  { std::cout << "constructing " << name << std::endl; }

  item (item const& other): name(other.name)
  { std::cout << "copy constructing " << name << std::endl; }

  ~item (void)
  { std::cout << "destroying " << name << std::endl; }
};

int main (void) {
  typedef compile_environment<item> env_type;
  typedef env_type::mapped_type mapped_type;

  char const* foo = "foo";
  boost::iterator_range<char const*> foo_rng(foo, foo + 3);

  mapped_type a;

  {
    env_type e;

    a = e.define(foo_rng, item("foo"));
  }

  SHEOL_TEST_EQ(a->name, "foo");

  return sheol::report_errors();
}

