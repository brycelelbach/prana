/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TREE_LESS_THAN_HPP)
#define BOOST_SPIRIT_PRANA_TREE_LESS_THAN_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/type_traits/is_arithmetic.hpp>

#include <boost/spirit/home/prana/utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct tree_less_than {
  typedef bool result_type;

  template<typename A, typename B>
  bool dispatch (const A&, const B&, boost::mpl::false_) const {
    return false; // cannot compare different types by default
  }

  template<typename A, typename B>
  bool dispatch (const A& a, const B& b, boost::mpl::true_) const {
    return a < b; // for arithmetic types
  }

  template<typename A, typename B>
  bool operator() (const A& a, const B& b) const {
    return dispatch(
      a, b,
      boost::mpl::and_<
        boost::is_arithmetic<A>,
        boost::is_arithmetic<B>
      >()
    );
  }

  template<typename T>
  bool operator() (const T& a, const T& b) const { return a < b; }

  template<typename Base, tree_type::info type_>
  bool operator() (
    basic_string<Base, type_> const& a,
    basic_string<Base, type_> const& b
  ) const {
    return static_cast<Base const&>(a) < static_cast<Base const&>(b);
  }

  bool operator() (nil, nil) const {
    BOOST_ASSERT(false); return false; 
  }
  
  bool operator() (any_ptr const& a, any_ptr const& b) const {
    BOOST_ASSERT(false); return false; 
  }

  bool operator() (function_base const& a, function_base const& b) const {
    BOOST_ASSERT(false); return false; 
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TREE_LESS_THAN_HPP
