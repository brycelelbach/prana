/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VISIT_HPP)
#define BOOST_SPIRIT_PRANA_VISIT_HPP

#include <boost/ref.hpp>
#include <boost/assert.hpp>
#include <boost/range/iterator_range.hpp>

#include <boost/mpl/if.hpp>

#include <boost/type_traits/is_const.hpp>

#include <boost/spirit/home/prana/tree_type.hpp>
#include <boost/spirit/home/prana/strings.hpp>
#include <boost/spirit/home/prana/pointers.hpp>
#include <boost/spirit/home/prana/bind.hpp>

namespace boost {
namespace spirit {
namespace prana {

// forward declaration
class utree;

template<typename UTreeX, typename UTreeY = UTreeX>
struct visitor {
 public:
  // single dispatch
  template<typename F> 
  typename F::result_type static dispatch (UTreeX& x, F f) { 
    typedef typename mpl::if_<
      boost::is_const<UTreeX>,
      typename UTreeX::const_iterator,
      typename UTreeX::iterator
    >::type iterator;

    typedef boost::iterator_range<iterator> list_range;

    switch (x.get_type()) {
      case tree_type::nil_type:
        nil x_;
        return f(x_);

      case tree_type::bool_type:
        return f(x.b);

      case tree_type::int_type:
        return f(x.i);

      case tree_type::double_type:
        return f(x.d);

      case tree_type::list_type:
        return f(list_range(iterator(x.l.first, 0), iterator(0, x.l.last)));

      case tree_type::range_type:
        return f(list_range(iterator(x.r.first, 0), iterator(0, x.r.last)));

      case tree_type::string_type:
        return f(utf8_string_range(x.s.str(), x.s.size()));

      case tree_type::string_range_type:
        return f(utf8_string_range(x.sr.first, x.sr.last));

      case tree_type::symbol_type:
        return f(utf8_symbol_range(x.s.str(), x.s.size()));

      case tree_type::binary_type:
        return f(binary_range(x.s.str(), x.s.size()));

      case tree_type::reference_type:
        return dispatch(*x.p, f);

      case tree_type::any_type:
        return f(any_ptr(x.v.p, x.v.i));

      case tree_type::function_type:
        return f(*x.pf);
    }
  }

  // double dispatch
  template<typename F>
  typename F::result_type static dispatch (UTreeX& x, UTreeY& y, F f) {
    typedef typename mpl::if_<
      is_const<UTreeX>,
      typename UTreeX::const_iterator,
      typename UTreeX::iterator
    >::type iterator;

    typedef iterator_range<iterator> list_range;

    switch (x.get_type()) {
      case tree_type::nil_type:
        nil x_;
        return dispatch(y, bind(f, x_));

      case tree_type::bool_type:
        return dispatch(y, bind(f, x.b));

      case tree_type::int_type:
        return dispatch(y, bind(f, x.i));

      case tree_type::double_type:
        return dispatch(y, bind(f, x.d));

      case tree_type::list_type:
        return dispatch(
          y, bind<F, list_range>(f, list_range(iterator
          (x.l.first, 0), iterator(0, x.l.last)))
        );

      case tree_type::range_type:
        return dispatch(
          y, bind<F, list_range>(f, list_range(iterator
          (x.r.first, 0), iterator(0, x.r.last)))
        );

      case tree_type::string_type:
       return dispatch(y, bind(f, utf8_string_range(x.s.str(), x.s.size())));

      case tree_type::string_range_type:
        return dispatch(y, bind(f, utf8_string_range(x.sr.first, x.sr.last)));

      case tree_type::symbol_type:
        return dispatch(y, bind(f, utf8_symbol_range(x.s.str(), x.s.size())));

      case tree_type::binary_type:
        return dispatch(y, bind(f, binary_range(x.s.str(), x.s.size())));

      case tree_type::reference_type:
        return dispatch(*x.p, y, f);

      case tree_type::any_type:
        return dispatch(y, bind(f, any_ptr(x.v.p, x.v.i)));

      case tree_type::function_type:
        return dispatch(y, bind(f, *x.pf));
    }
  }
};

template<typename F>
typename F::result_type visit (utree const& x, F f) {
  return visitor<utree const>::dispatch(x, f);
}

template<typename F>
typename F::result_type visit (utree& x, F f) {
  return visitor<utree>::dispatch(x, f);
}

template<typename F>
typename F::result_type visit (utree const& x, utree const& y, F f) {
  return visitor<utree const, utree const>::dispatch(x, y, f);
}

template<typename F>
typename F::result_type visit (utree const& x, utree& y, F f) {
  return visitor<utree const, utree>::dispatch(x, y, f);
}

template<typename F>
typename F::result_type visit (utree& x, utree const& y, F f) {
  return visitor<utree, utree const>::dispatch(x, y, f);
}

template<typename F>
typename F::result_type visit (utree& x, utree& y, F f) {
  return visitor<utree, utree>::dispatch(x, y, f);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VISIT_HPP
