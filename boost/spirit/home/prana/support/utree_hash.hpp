/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SUPPORT_UTREE_HASH_HPP)
#define BOOST_SPIRIT_PRANA_SUPPORT_UTREE_HASH_HPP

#include <boost/functional/hash.hpp>

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct utree_hasher {
  typedef std::size_t result_type;

  result_type operator() (utree::uninitialized_type) const {
    return hash_value(utree_type::uninitialized_type);
  }

  result_type operator() (utree::nil_type) const {
    return hash_value(utree_type::nil_type);
  }

  result_type operator() (int i) const {
    std::size_t seed = 0;
    hash_combine(seed, utree_type::int_type);
    hash_combine(seed, i); 
    return seed;
  }

  result_type operator() (double d) const {
    std::size_t seed = 0;
    hash_combine(seed, utree_type::double_type);
    hash_combine(seed, d); 
    return seed;
  }

  result_type operator() (bool b) const {
    std::size_t seed = 0;
    hash_combine(seed, utree_type::bool_type);
    hash_combine(seed, b);
    return seed;
  }

  result_type operator() (binary_range_type const& str) const {
    std::size_t seed = 0;
    hash_combine(seed, utree_type::binary_type);
    hash_combine(seed, hash_range(str.begin(), str.end()));
    return seed;
  }

  result_type operator() (utf8_string_range_type const& str) const {
    std::size_t seed = 0;
    hash_combine(seed, utree_type::string_type);
    hash_combine(seed, hash_range(str.begin(), str.end()));
    return seed;
  }

  result_type operator() (utf8_symbol_range_type const& str) const {
    std::size_t seed = 0;
    hash_combine(seed, utree_type::symbol_type);
    hash_combine(seed, hash_range(str.begin(), str.end()));
    return seed;
  }

  template<typename Iterator>
  result_type operator() (iterator_range<Iterator> const& range) const {
    std::size_t seed = 0;
    hash_combine(seed, utree_type::range_type);
    hash_combine(seed, hash_range(range.begin(), range.end()));
    return seed;
  }

  result_type operator() (any_ptr const& p) const {
    std::size_t seed = 0;
    hash_combine(seed, utree_type::any_type);
    hash_combine(seed, &p); 
    return seed;
  }

  result_type operator() (function_base const& pf) const {
    std::size_t seed = 0;
    hash_combine(seed, utree_type::function_type);
    hash_combine(seed, &pf); 
    return seed;
  }
};

std::size_t hash_value (utree const& ut) {
  return utree::visit(ut, utree_hasher());
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SUPPORT_UTREE_HASH_HPP

