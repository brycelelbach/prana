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

#include "prana_test_harness.hpp"

using namespace boost::spirit::prana;

BOOST_AUTO_TEST_CASE(default_ctor) {
  utree<> t;
}

