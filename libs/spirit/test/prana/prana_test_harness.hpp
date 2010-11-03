/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TEST_HARNESS_HPP)
#define BOOST_SPIRIT_PRANA_TEST_HARNESS_HPP

#include <ctime>

#include <boost/mpl/list.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/identity.hpp>
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
  boost::mpl::identity<short>, boost::mpl::identity<signed>,
  boost::mpl::identity<unsigned>, boost::mpl::identity<double>,
  string<2>, string<4>, string<8>, string<16>, string<32>
> rounds;

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
