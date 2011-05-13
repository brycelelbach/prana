////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 1999, 2000 Jaakko Jarvi
//  Copyright (c) 2001, 2002 Peter Dimov
//  Copyright (c) 2002 David Abrahams
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PRANA_79635E14_1E51_4747_9AE6_FAE225085715)
#define PRANA_79635E14_1E51_4747_9AE6_FAE225085715

#include <boost/config.hpp>
#include <boost/utility/result_of.hpp>

namespace boost {
namespace spirit {
namespace prana {

template <typename F>
struct stateful_visitor { 
  typedef F type;

  template <typename> struct result;

  template <typename This>
  struct result<This(void)> {
    typedef typename boost::result_of<F(void)>::type type;
  };
 
  template <typename This, typename X>
  struct result<This(X)> {
    typedef typename boost::result_of<F(X)>::type type;
  };
  
  template <typename This, typename X, typename Y>
  struct result<This(X, Y)> {
    typedef typename boost::result_of<F(X, Y)>::type type;
  };

  explicit stateful_visitor (F& f_): f(&f_) { }

  operator F& (void) const
  { return *f; }

  F& get (void) const
  { return *f; }

  F* get_pointer (void) const
  { return f; }

  typename result<stateful_visitor(void)>::type
  operator() (void) const
  { return get()(); }

  template <typename X>
  typename result<stateful_visitor(X)>::type
  operator() (X x) const
  { return get()(x); }

  template <typename X, typename Y>
  typename result<stateful_visitor(X, Y)>::type
  operator() (X x, Y y) const
  { return get()(x, y); }

 private:
  F* f;
};

template <typename F>
inline stateful_visitor<F> visit_ref (F& t)
{ return stateful_visitor<F>(t); }

template <typename F>
inline stateful_visitor<F const> visit_cref (F const& t)
{ return stateful_visitor<F const>(t); }

} // prana
} // spirit
} // boost

#endif // PRANA_79635E14_1E51_4747_9AE6_FAE225085715

