/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_TEST_MODULE prana_utree_core

#include <cstddef>

#include <vector>
#include <list>

#include <boost/spirit/home/prana/utree.hpp>
#include <boost/spirit/home/prana/functional/default_asserter.hpp>

#include "prana_test_harness.hpp"

using namespace boost::spirit::prana;

typedef utree<utree_policy<char, functional::default_asserter> > char_utree;
typedef utree<utree_policy<wchar_t, functional::default_asserter> > wchar_utree;

BOOST_AUTO_TEST_CASE(ctors_and_dtors) {
  char_utree s;
  wchar_utree w;
  BOOST_CHECK(sizeof(char_utree) == sizeof(wchar_utree));
}
