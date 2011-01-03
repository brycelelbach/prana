/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "harness.hpp"

#include <boost/spirit/home/prana/adt/ternary_search_tree.hpp> 

int main (void) { try {
  using boost::spirit::prana::ternary_search_tree;

  typedef ternary_search_tree<char, unsigned> tst_type;

  // iterator interface
  { 
    tst_type t;

    std::string abc("abc"), def("def"), abcdef("abcdef");

    // insertion
    t.insert(abc.begin(), abc.end(), 1);
    t.insert(def.begin(), def.end(), 2);
    t.insert(abcdef.begin(), abcdef.end(), 3);

    // lookup
    std::string::iterator it = abc.begin();
    BOOST_TEST(bool(t.find(it, abc.end())) == true);
    BOOST_TEST(it == abc.end());
 
    it = def.begin();
    BOOST_TEST(bool(t.find(it, def.end())) == true);
    BOOST_TEST(it == def.end());
 
    it = abcdef.begin();
    BOOST_TEST(bool(t.find(it, abcdef.end())) == true);
    BOOST_TEST(it == abcdef.end());
 
    // removal
    t.erase(abc.begin(), abc.end());
    it = abc.begin();
    BOOST_TEST(bool(t.find(it, abc.end())) == false);
 
    t.erase(def.begin(), def.end());
    it = def.begin();
    BOOST_TEST(bool(t.find(it, def.end())) == false);

    // verify data
    t.insert(abc.begin(), abc.begin() + 1, 1);
    it = abc.begin();
    tst_type::pointer r = t.find(it, abc.end());
    BOOST_TEST(bool(r) == true);
    BOOST_TEST(*r == 1);
    BOOST_TEST(it != abc.end());
 
    t.insert(def.begin(), def.end(), 2);
    it = def.begin();
    r = t.find(it, def.end());
    BOOST_TEST(bool(r) == true);
    BOOST_TEST(*r == 2);
    BOOST_TEST(it == def.end());

    it = abcdef.begin();
    r = t.find(it, abcdef.end());
    BOOST_TEST(bool(r) == true);
    BOOST_TEST(*r == 3);
    BOOST_TEST(it == abcdef.end());
  }
  
  // container interface
  { 
    tst_type t;

    std::string abc("abc"), def("def"), abcdef("abcdef");

    // insertion
    t.insert(abc, 1);
    t.insert(def, 2);
    t.insert(abcdef, 3);

    // lookup
    BOOST_TEST(t.find(abc));
    BOOST_TEST(t.find(def));
    BOOST_TEST(t.find(abcdef));

    // verify data
    BOOST_TEST(t[abc] == 1);
    BOOST_TEST(t[def] == 2);
    BOOST_TEST(t[abcdef] == 3);
 
    // removal
    t.erase(abc);
    t.erase(def);
    t.erase(abcdef);
    BOOST_TEST(!t.find(abc));
    BOOST_TEST(!t.find(def));
    BOOST_TEST(!t.find(abcdef));
  }
  
  // c-string interface
  { 
    tst_type t;

    // insertion
    t.insert("abc", 1);
    t.insert("def", 2);
    t.insert("abcdef", 3);

    // lookup
    BOOST_TEST(t.find("abc"));
    BOOST_TEST(t.find("def"));
    BOOST_TEST(t.find("abcdef"));

    // verify data
    BOOST_TEST(t["abc"] == 1);
    BOOST_TEST(t["def"] == 2);
    BOOST_TEST(t["abcdef"] == 3);
 
    // removal
    t.erase("abc");
    t.erase("def");
    t.erase("abcdef");
    BOOST_TEST(!t.find("abc"));
    BOOST_TEST(!t.find("def"));
    BOOST_TEST(!t.find("abcdef"));
  }

  } catch (std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
    return -1;
  }

  return boost::report_errors();
}

