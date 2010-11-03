/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_UNIT_TEST_DATA_GENERATOR)
#define BOOST_UNIT_TEST_DATA_GENERATOR

#include <ctime>

#include <list>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/identity.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

namespace boost {
namespace unit_test {

template<
  std::size_t Length = 8,
  template<typename, typename> class Container = std::list,
  std::size_t Elements = 4
>
struct string: boost::mpl::identity<std::string> {
  typedef Container<std::string, std::allocator<std::string> > container;
  typedef boost::mpl::size_t<Elements> elements;
  typedef boost::mpl::size_t<Length> length;
};

template<
  template<typename, typename> class Container = std::list,
  std::size_t Elements = 4
>
struct integer: boost::mpl::identity<int> {
  typedef Container<int, std::allocator<int> > container;
  typedef boost::mpl::size_t<Elements> elements;
};

template<
  template<typename, typename> class Container = std::list,
  std::size_t Elements = 4
>
struct floating: boost::mpl::identity<double> {
  typedef Container<double, std::allocator<double> > container;
  typedef boost::mpl::size_t<Elements> elements;
};

struct current_time {
  typedef std::size_t result_type;

  std::size_t operator() (void) const;
};

inline std::size_t current_time::operator() (void) const {
  return std::time(0);
}

template<
  typename Generator = boost::random::mt19937,
  typename IntDistribution = boost::uniform_int<>,
  typename RealDistribution = boost::uniform_real<>,
  typename DefaultSeed = current_time
>
class data_generator {
 private:
  std::size_t _seed; 
  Generator   _generator;

  IntDistribution  _int_dist;
  RealDistribution _real_dist;

 public:
  data_generator (void):
    _seed(DefaultSeed()()),
    _generator(_seed) {
      // EXPLAIN (wash): Leave a line here so we can breakpoint the ctor
    }

  data_generator (std::size_t seed_):
    _seed(seed_),
    _generator(_seed) {
      // EXPLAIN (wash): Leave a line here so we can breakpoint the ctor
    }

  std::size_t seed (void) const {
    return _seed;
  }

  template<typename T>
  typename boost::enable_if<
    boost::is_floating_point<typename T::type>,
    typename T::type
  >::type get (void) { 
    boost::variate_generator<Generator&, RealDistribution>
      gen(_generator, _real_dist);
    return gen();
  }

  template<typename T>
  typename boost::enable_if<
    boost::is_integral<typename T::type>,
    typename T::type
  >::type get (void) {
    boost::variate_generator<Generator&, IntDistribution>
      gen(_generator, _int_dist);
    return gen();
  }
  
  template<typename T>
  typename boost::enable_if<
    boost::is_same<typename T::type, std::string>,
    typename T::type
  >::type get (void) {
    static char const chars[] = 
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

    boost::variate_generator<Generator&, boost::uniform_int<> >
      gen(_generator, boost::uniform_int<>(0, 61));

    std::string r;
    for (unsigned i = 0; i < T::length::value; ++i) { r += chars[gen()]; }
    return r;
  }
};

} // unit_test
} // boost

#endif // BOOST_UNIT_TEST_DATA_GENERATOR
