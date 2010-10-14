/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_dllist

#include <boost/spirit/home/prana/adt/dllist.hpp>
#include <boost/test/unit_test.hpp>

using namespace boost::spirit::prana;

BOOST_AUTO_TEST_CASE(default_and_free) {
  dllist<int> l;
  l.default_construct();
  l.free();
}

BOOST_AUTO_TEST_CASE(insertion) {
  dllist<int> l;
  l.default_construct();
  l.push_back(15);
  l.push_back(2134);
  l.push_back(-423);
  l.free();
}

