#include <iostream>

#include <boost/detail/lightweight_test.hpp>

#include <phxpr/environment.hpp>

using phxpr::environment;

struct object {
  std::string name;

  object (std::string const& name_ = "#<anonymous>"): name(name_)
  { std::cout << "constructing " << name << std::endl; }

  object (object const& other): name(other.name)
  { std::cout << "copy constructing " << name << std::endl; }

  ~object (void)
  { std::cout << "destroying " << name << std::endl; }
};

int main (void) {
  typedef environment<object> env_type;
  typedef env_type::mapped_type mapped_type;

  char const* foo = "foo";
  boost::iterator_range<char const*> foo_rng(foo, foo + 3);

  mapped_type a;

  {
    env_type e;

    a = e.define(foo_rng, object("foo"));
  }

  BOOST_TEST_EQ(a->name, "foo");

  return boost::report_errors();
}

