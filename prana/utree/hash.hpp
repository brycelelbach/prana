/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_DBF60B12_8846_4B46_B883_11FBF7BF3ECC)
#define PRANA_DBF60B12_8846_4B46_B883_11FBF7BF3ECC

#include <prana/config.hpp>

#include <boost/functional/hash.hpp>

#include <prana/include/utree.hpp>

namespace prana {

struct utree_hasher {
  typedef std::size_t result_type;

  result_type operator() (utree::invalid_type) const {
    return boost::hash_value(utree_type::invalid_type);
  }

  result_type operator() (utree::nil_type) const {
    return boost::hash_value(utree_type::nil_type);
  }

  result_type operator() (int i) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, utree_type::int_type);
    boost::hash_combine(seed, i); 
    return seed;
  }

  result_type operator() (double d) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, utree_type::double_type);
    boost::hash_combine(seed, d); 
    return seed;
  }

  result_type operator() (bool b) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, utree_type::bool_type);
    boost::hash_combine(seed, b);
    return seed;
  }

  result_type operator() (spirit::binary_range_type const& str) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, utree_type::binary_type);
    boost::hash_combine(seed, boost::hash_range(str.begin(), str.end()));
    return seed;
  }

  result_type operator() (spirit::utf8_string_range_type const& str) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, utree_type::string_type);
    boost::hash_combine(seed, boost::hash_range(str.begin(), str.end()));
    return seed;
  }

  result_type operator() (spirit::utf8_symbol_range_type const& str) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, utree_type::symbol_type);
    boost::hash_combine(seed, boost::hash_range(str.begin(), str.end()));
    return seed;
  }

  template <typename Iterator>
  result_type operator() (boost::iterator_range<Iterator> const& range) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, utree_type::range_type);
    boost::hash_combine(seed, boost::hash_range(range.begin(), range.end()));
    return seed;
  }

  result_type operator() (spirit::any_ptr const& p) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, utree_type::any_type);
    boost::hash_combine(seed, &p); 
    return seed;
  }

  result_type operator() (spirit::function_base const& pf) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, utree_type::function_type);
    boost::hash_combine(seed, &pf); 
    return seed;
  }
};

} // prana

namespace boost {
namespace spirit {

std::size_t hash_value (utree const& ut) {
  return utree::visit(ut, prana::utree_hasher());
}

} // spirit
} // boost

#endif // PRANA_DBF60B12_8846_4B46_B883_11FBF7BF3ECC

