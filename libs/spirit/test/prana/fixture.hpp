/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TEST_HARNESS_HPP)
#define BOOST_SPIRIT_PRANA_TEST_HARNESS_HPP

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_LIST_SIZE 40

#include <cstring>
#include <cstddef>

#include <list>
#include <vector>

#include <boost/mpl/list.hpp>
#include <boost/mpl/list_c.hpp>

#include <boost/test/unit_test.hpp>

#include <data_generator.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace test {

using namespace boost::unit_test;
using namespace boost::mpl;

struct fixture {
  data_generator<> generator;

  fixture (void) {
    unit_test_log.set_threshold_level(log_successful_tests);
    BOOST_TEST_MESSAGE("seed: " << generator.seed()); 
  }
};

} // test
} // prana 
} // spirit
} // boost 

#endif // BOOST_SPIRIT_PRANA_TEST_HARNESS_HPP
