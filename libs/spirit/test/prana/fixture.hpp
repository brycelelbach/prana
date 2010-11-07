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
#include <boost/test/unit_test.hpp>

#include <data_generator.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace test {

using namespace boost::unit_test;

typedef boost::mpl::list<
  integer<0>,   floating<0>,   string<0, 4>,   string<0, 18>,
  integer<1>,   floating<1>,   string<1, 4>,   string<1, 18>,
  integer<2>,   floating<2>,   string<2, 4>,   string<2, 18>,
  integer<4>,   floating<4>,   string<4, 4>,   string<4, 18>,
  integer<8>,   floating<8>,   string<8, 4>,   string<8, 18>,
  integer<16>,  floating<16>,  string<16, 4>,  string<16, 18>,
  integer<32>,  floating<32>,  string<32, 4>,  string<32, 18>,
  integer<64>,  floating<64>,  string<64, 4>,  string<64, 18>
> elements;

typedef boost::mpl::list<
  integer<4, std::list>,      floating<4, std::list>,
  string<4, 4, std::list>,    string<4, 18, std::list>,
  integer<8, std::list>,      floating<8, std::list>,
  string<8, 4, std::list>,    string<8, 18, std::list>,
  integer<16, std::list>,     floating<16, std::list>,
  string<16, 4, std::list>,   string<16, 18, std::list>,
  integer<32, std::list>,     floating<32, std::list>,
  string<32, 4, std::list>,   string<32, 18, std::list>,
  integer<4, std::vector>,    floating<4, std::vector>,
  string<4, 4, std::vector>,  string<4, 18, std::vector>,
  integer<8, std::vector>,    floating<8, std::vector>,
  string<8, 4, std::vector>,  string<8, 18, std::vector>,
  integer<16, std::vector>,   floating<16, std::vector>,
  string<16, 4, std::vector>, string<16, 18, std::vector>,
  integer<32, std::vector>,   floating<32, std::vector>,
  string<32, 4, std::vector>, string<32, 18, std::vector>
> containers;

typedef boost::mpl::list<
  string<1, 1>,  string<1, 2>,  string<1, 3>,  string<1, 4>,
  string<1, 5>,  string<1, 6>,  string<1, 7>,  string<1, 8>,
  string<1, 9>,  string<1, 10>, string<1, 11>, string<1, 12>,
  string<1, 13>, string<1, 14>, string<1, 15>, string<1, 16>,
  string<1, 17>, string<1, 18>, string<1, 19>, string<1, 20>,
  string<1, 21>, string<1, 22>, string<1, 23>, string<1, 24>,
  string<1, 25>, string<1, 26>, string<1, 27>, string<1, 28>,
  string<1, 29>, string<1, 30>, string<1, 31>, string<1, 32> 
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
