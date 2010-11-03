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

#include <ctime>

#include <boost/mpl/list.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/cstdint.hpp>
#include <boost/random.hpp>
#include <boost/integer_traits.hpp>
#include <boost/type_traits.hpp>
#include <boost/test/unit_test.hpp>

#if !defined(BOOST_SPIRIT_PRANA_TEST_SEED)
  #define BOOST_SPIRIT_PRANA_TEST_SEED std::time(0)
#endif 

namespace boost {
namespace spirit {
namespace prana {
namespace test {

template<std::size_t N>
struct string: boost::mpl::size_t<N> {
  typedef std::string type;
};

typedef boost::mpl::list<
  boost::mpl::identity<boost::int8_t>, boost::mpl::identity<boost::uint8_t>,
  boost::mpl::identity<boost::int16_t>, boost::mpl::identity<boost::uint16_t>,
  boost::mpl::identity<boost::int32_t>, boost::mpl::identity<boost::uint32_t>,
  boost::mpl::identity<float>, boost::mpl::identity<double>,
  string<2>, string<4>, string<6>, string<8>,
  string<16>, string<32>, string<64>, string<128>
> rounds;

typedef boost::mpl::list<
  string<1>, string<2>, string<3>, string<4>, string<5>,
  string<6>, string<7>, string<8>, string<9>, string<10>,
  string<11>, string<12>, string<13>, string<14>, string<15>,
  string<16>, string<17>, string<18>, string<19>, string<20>,
  string<21>, string<22>, string<23>, string<24>, string<25>,
  string<26>, string<27>, string<28>, string<29>, string<30>,
  string<31>, string<32> 
> strings;

struct fixture {
  std::size_t seed; 
  boost::random::mt19937 generator;
  std::string const chars;

  fixture (void):
    seed(BOOST_SPIRIT_PRANA_TEST_SEED),
    generator(seed),
    chars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890")
  {
    using namespace boost::unit_test;
    unit_test_log.set_threshold_level(log_successful_tests);
    BOOST_TEST_MESSAGE("seed: " << seed); 
  }

  template<typename T>
  typename boost::enable_if<
    boost::is_floating_point<typename T::type>,
    typename T::type
  >::type random (void) {
    return ((typename T::type) generator()) / ((typename T::type) generator());
  }

  template<typename T>
  typename boost::enable_if<
    boost::is_integral<typename T::type>,
    typename T::type
  >::type random (void) {
    return (typename T::type) generator();
  }
  
  template<typename T>
  typename boost::enable_if<
    boost::is_same<typename T::type, std::string>,
    typename T::type
  >::type random (void) {
    boost::variate_generator<boost::random::mt19937&, boost::uniform_int<> >
      gen(generator, boost::uniform_int<>(0, chars.size() - 1));

    std::string r;

    for (unsigned i = 0; i < T::value; ++i) { r += chars[gen()]; }

    return r;
  }
};

} // test
} // prana 
} // spirit
} // boost 

#endif // BOOST_SPIRIT_PRANA_TEST_HARNESS_HPP
