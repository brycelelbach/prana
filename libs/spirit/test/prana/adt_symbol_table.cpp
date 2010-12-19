/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/adt/symbol_table.hpp> 

int main (void) {
  using namespace boost;
  using namespace boost::spirit;

  typedef prana::symbol_table<char, unsigned> table_type;
  
  table_type t;

  std::string foo("foo"), bar("bar"), foobar("foobar");
  unsigned a(17), b(1405), c(742);

  //[insertion
  t.insert(foo.begin(), foo.end(), a);
  t.insert(bar.begin(), bar.end(), b);
  t.insert(foobar.begin(), foobar.end(), c);
  //]

  //[find_foo
  std::string::iterator it = foo.begin();

  BOOST_TEST(bool(t.find(it, foo.end())) == true);
  BOOST_TEST(it == foo.end());
  //] 
 
  //[find_bar
  it = bar.begin();

  BOOST_TEST(bool(t.find(it, bar.end())) == true);
  BOOST_TEST(it == bar.end());
  //]
 
  //[find_foobar 
  it = foobar.begin();

  BOOST_TEST(bool(t.find(it, foobar.end())) == true);
  BOOST_TEST(it == foobar.end());
  //]
 
  //[erase_foo
  t.erase(foo.begin(), foo.end());
  it = foo.begin();

  BOOST_TEST(bool(t.find(it, foo.end())) == false);
  //]
 
  //[erase_bar 
  t.erase(bar.begin(), bar.end());
  it = bar.begin();

  BOOST_TEST(bool(t.find(it, bar.end())) == false);
  //]

  //[test_foo_data
  t.insert(foo.begin(), foo.begin() + 1, a);
  it = foo.begin();
  table_type::pointer r = t.find(it, foo.end());

  BOOST_TEST(bool(r) == true);
  BOOST_TEST(*(fusion::at_c<0>(*r)->begin()) == *foo.begin());
  BOOST_TEST(*fusion::at_c<1>(*r) == a);
  BOOST_TEST(*it == *(foo.begin() + 1));
  //]
 
  //[test_bar_data 
  t.insert(bar.begin(), bar.end(), b);
  it = bar.begin();
  r = t.find(it, bar.end());

  BOOST_TEST(bool(r) == true);
  BOOST_TEST(*(fusion::at_c<0>(*r)->begin()) == *bar.begin());
  BOOST_TEST(*(fusion::at_c<0>(*r)->end()) == *it);
  BOOST_TEST(*fusion::at_c<1>(*r) == b);
  BOOST_TEST(*it == *bar.end());
  //]

  //[test_foobar_data 
  it = foobar.begin();
  r = t.find(it, foobar.end());

  BOOST_TEST(bool(r) == true);
  BOOST_TEST(*(fusion::at_c<0>(*r)->begin()) == *foobar.begin());
  BOOST_TEST(*(fusion::at_c<0>(*r)->end()) == *it);
  BOOST_TEST(*fusion::at_c<1>(*r) == c);
  BOOST_TEST(*it == *foobar.end());
  //]

  return boost::report_errors();
}

