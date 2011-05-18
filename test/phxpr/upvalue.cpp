#include <iostream>

#include <boost/detail/lightweight_test.hpp>

#include <phxpr/upvalue.hpp>

using phxpr::upvalue0;
using phxpr::upvalue1;
using phxpr::upvalue2;
using phxpr::upvalue3;
using phxpr::upvalue4;
using phxpr::upvalue5;
using phxpr::upvalue6;
using phxpr::upvalue7;
using phxpr::upvalue8;

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
  upvalue0<object> up0;
  upvalue1<object, object> up1;
  upvalue2<object, object, object> up2;
  upvalue3<object, object, object, object> up3;
  upvalue4<object, object, object, object, object> up4;
  upvalue5<object, object, object, object, object, object> up5;
  upvalue6<object, object, object, object, object, object, object> up6;
  upvalue7<object, object, object, object, object, object, object, object> up7;
  upvalue8<object, object, object, object, object, object, object, object, object> up8;

  BOOST_TEST(true);

  return boost::report_errors();
}

