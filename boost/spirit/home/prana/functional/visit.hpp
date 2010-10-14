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

#include <boost/mpl/if.hpp>

#include <boost/type_traits/is_const.hpp>

#include <boost/spirit/home/prana/adt/typed_string.hpp>
#include <boost/spirit/home/prana/adt/fast_string.hpp>
#include <boost/spirit/home/prana/adt/any_ptr.hpp>
#include <boost/spirit/home/prana/adt/irange.hpp>

#include <boost/spirit/home/prana/functional/bind.hpp>

#include <boost/spirit/home/prana/tree_type.hpp>
#include <boost/spirit/home/prana/domain.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct visitor {
 public:
  template<typename, typename, typename = prana::unused_type> struct result;

  template<typename TreeX, typename TreeY, typename F>
  struct result { typedef typename F::result_type type; };
  
  template<typename TreeX, typename F>
  struct result<TreeX, F, prana::unused_type> {
    typedef typename F::result_type type;
  };

  // single (*this)
  template<typename TreeX, typename F> 
  typename F::result_type operator() (TreeX& x, F f) const { 
    typedef typename mpl::if_<
      boost::is_const<TreeX>,
      typename TreeX::const_iterator,
      typename TreeX::iterator
    >::type iterator;

    typedef irange<iterator> list_range;

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
        return f(list_range::make(
          iterator(x.l.first, 0), iterator(0, x.l.last))
        );

      case tree_type::range_type:
        return f(list_range::make(
          iterator(x.r.first, 0), iterator(0, x.r.last))
        );

      case tree_type::string_type:
        return f(utf8_string_range::make(
            x.s.str(), x.s.size()
          )
        );

      case tree_type::string_range_type:
        return f(utf8_string_range::make(
            x.sr.first, x.sr.last
          )
        );

      case tree_type::symbol_type:
        return f(utf8_symbol_range::make(x.s.str(), x.s.size()));

      case tree_type::binary_type:
        return f(binary_range::make(x.s.str(), x.s.size()));

      case tree_type::reference_type:
        return (*this)(*x.p, f);

      case tree_type::any_type:
        return f(x.p);

      case tree_type::record_type:
        return f(*x.pf);
    }
  }

  // double (*this)
  template<typename TreeX, typename TreeY, typename F>
  typename F::result_type operator() (TreeX& x, TreeY& y, F f) const {
    typedef typename mpl::if_<
      is_const<TreeX>,
      typename TreeX::const_iterator,
      typename TreeX::iterator
    >::type iterator;

    typedef irange<iterator> list_range;

    switch (x.get_type()) {
      case tree_type::nil_type:
        nil x_;
        return (*this)(y, bind(f, x_));

      case tree_type::bool_type:
        return (*this)(y, bind(f, x.b));

      case tree_type::int_type:
        return (*this)(y, bind(f, x.i));

      case tree_type::double_type:
        return (*this)(y, bind(f, x.d));

      case tree_type::list_type:
        return (*this)(
          y, bind<F, list_range>(f, list_range::make(
            iterator(x.l.first, 0), iterator(0, x.l.last))
          )
        );

      case tree_type::range_type:
        return (*this)(
          y, bind<F, list_range>(f, list_range::make(
            iterator(x.r.first, 0), iterator(0, x.r.last))
          )
        );

      case tree_type::string_type:
        return (*this)(y, bind(f, utf8_string_range::make(
          x.s.str(), x.s.size()))
        );

      case tree_type::string_range_type:
        return (*this)(y, bind(f, utf8_string_range::make(
          x.sr.first, x.sr.last))
        );

      case tree_type::symbol_type:
        return (*this)(y, bind(f, utf8_symbol_range::make(
          x.s.str(), x.s.size()))
        );

      case tree_type::binary_type:
        return (*this)(y, bind(f, binary_range::make(
          x.s.str(), x.s.size()))
        );

      case tree_type::reference_type:
        return (*this)(*x.p, y, f);

      case tree_type::any_type:
        return (*this)(y, bind(f, x.v));

      case tree_type::record_type:
        return (*this)(y, bind(f, *x.pf));
    }
  }
};

template<typename TreeX, typename F>
typename visitor::result<TreeX&, F, prana::unused_type>::type
visit (TreeX& x, F f) {
  visitor v;
  return v(x, f);
}

template<typename TreeX, typename TreeY, typename F>
typename visitor::result<TreeX&, TreeY&, F>::type
visit (TreeX& x, TreeY& y, F f) {
  visitor v;
  return v(x, y, f);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VISIT_HPP
