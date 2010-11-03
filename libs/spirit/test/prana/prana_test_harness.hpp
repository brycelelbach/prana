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

#include <vector>

#include <boost/mpl/list.hpp>

#include <boost/test/unit_test.hpp>

#include "data_generator.hpp"

namespace boost {
namespace spirit {
namespace prana {
namespace test {

using namespace boost::unit_test;

typedef boost::mpl::list<
  integer<std::list, 4>,      floating<std::list, 4>,
  string<4, std::list, 4>,    string<18, std::list, 4>,
  integer<std::list, 8>,      floating<std::list, 8>,
  string<4, std::list, 8>,    string<18, std::list, 8>,
  integer<std::list, 16>,     floating<std::list, 16>,
  string<4, std::list, 16>,   string<18, std::list, 16>,
  integer<std::list, 32>,     floating<std::list, 32>,
  string<4, std::list, 32>,   string<18, std::list, 32>,
  integer<std::vector, 4>,    floating<std::vector, 4>,
  string<4, std::vector, 4>,  string<18, std::vector, 4>,
  integer<std::vector, 8>,    floating<std::vector, 8>,
  string<4, std::vector, 8>,  string<18, std::vector, 8>,
  integer<std::vector, 16>,   floating<std::vector, 16>,
  string<4, std::vector, 16>, string<18, std::vector, 16>,
  integer<std::vector, 32>,   floating<std::vector, 32>,
  string<4, std::vector, 32>, string<18, std::vector, 32>
> containers;

typedef boost::mpl::list<
  string<1>,  string<2>,  string<3>,  string<4>,
  string<5>,  string<6>,  string<7>,  string<8>,
  string<9>,  string<10>, string<11>, string<12>,
  string<13>, string<14>, string<15>, string<16>,
  string<17>, string<18>, string<19>, string<20>,
  string<21>, string<22>, string<23>, string<24>,
  string<25>, string<26>, string<27>, string<28>,
  string<29>, string<30>, string<31>, string<32> 
> strings;

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
