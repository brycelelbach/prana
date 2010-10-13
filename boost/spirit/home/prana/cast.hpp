/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_CAST_HPP)
#define BOOST_SPIRIT_PRANA_CAST_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_convertible.hpp>

#include <boost/spirit/home/prana/adt/any_ptr.hpp>
#include <boost/spirit/home/prana/exceptions.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename T>
struct is_iterator_range: mpl::false_ { };

template<typename Iterator>
struct is_iterator_range<boost::iterator_range<Iterator> >: mpl::true_ { };

template<typename To>
struct cast {
  typedef To result_type;

  template<typename From>
  To dispatch (From const& val, mpl::true_) const {
    return To(val); // From is convertible to To
  }

  template<typename From>
  To dispatch (From const& val, mpl::false_) const {
    // From is NOT convertible to To !!!
    throw bad_cast();
    return To();
  }

  template<typename From>
  To operator() (From const& val) const {
    // boost::iterator_range has a templated constructor, accepting
    // any argument and hence any type is 'convertible' to it.
    typedef typename mpl::eval_if<
      is_iterator_range<To>,
      boost::is_same<From, To>,
      boost::is_convertible<From, To>
    >::type is_convertible;
    return dispatch(val, is_convertible());
  }
};

template <typename T>
struct cast<T*> {
  typedef T* result_type;

  template<typename From>
  T* operator() (From const& val) const {
    // From is NOT convertible to T !!!
    throw bad_cast();
    return 0;
  }

  T* operator() (any_ptr const& p) const {
    return p.get<T*>();
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_CAST_HPP

