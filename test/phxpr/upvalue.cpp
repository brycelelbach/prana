////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <sheol/lightweight_test.hpp>

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
  upvalue0<item> up0;
  upvalue1<item, item> up1;
  upvalue2<item, item, item> up2;
  upvalue3<item, item, item, item> up3;
  upvalue4<item, item, item, item, item> up4;
  upvalue5<item, item, item, item, item, item> up5;
  upvalue6<item, item, item, item, item, item, item> up6;
  upvalue7<item, item, item, item, item, item, item, item> up7;
  upvalue8<item, item, item, item, item, item, item, item, item> up8;

  return sheol::report_errors();
}

