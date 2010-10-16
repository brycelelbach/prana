/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TEST_HARNESS_HPP)
#define BOOST_SPIRIT_PRANA_TEST_HARNESS_HPP

#include <boost/test/unit_test.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct log_config {
  log_config (void) {
    using namespace boost::unit_test;
    unit_test_log.set_threshold_level(log_messages);
  }
}; 

BOOST_GLOBAL_FIXTURE(log_config);

} // prana 
} // spirit
} // boost 

#endif // BOOST_SPIRIT_PRANA_TEST_HARNESS_HPP
