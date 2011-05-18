////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PRANA_79635E14_1E51_4747_9AE6_FAE225085715)
#define PRANA_79635E14_1E51_4747_9AE6_FAE225085715

#include <prana/config.hpp>

#include <boost/utility/result_of.hpp>

namespace prana {

template <typename F>
struct stateful_visitor { 
  typedef F type;

  #if !defined(PRANA_NO_VISIT_REF_RESULT_TEMPLATE)
    template <typename> struct result;
  
    template <typename This>
    struct result<This(void)> {
      typedef typename boost::result_of<F(void)>::type type;
    };
   
    template <typename This, typename X>
    struct result<This(X&)> {
      typedef typename boost::result_of<F(X&)>::type type;
    };
    
    template <typename This, typename X, typename Y>
    struct result<This(X&, Y&)> {
      typedef typename boost::result_of<F(X&, Y&)>::type type;
    };
  #else
    typedef typename F::result_type result_type;
  #endif

  explicit stateful_visitor (F& f_): f(&f_) { }

  operator F& (void) const
  { return *f; }

  F& get (void) const
  { return *f; }

  F* get_pointer (void) const
  { return f; }

  #if !defined(PRANA_NO_VISIT_REF_RESULT_TEMPLATE)
    typename result<stateful_visitor(void)>::type
  #else
    result_type
  #endif
  operator() (void) const
  { return get()(); }

  // {{{ unary
  template <typename X>
  #if !defined(PRANA_NO_VISIT_REF_RESULT_TEMPLATE)
    typename result<stateful_visitor(X&)>::type
  #else
    result_type
  #endif
  operator() (X& x) const
  { return get()(x); }
  
  template <typename X>
  #if !defined(PRANA_NO_VISIT_REF_RESULT_TEMPLATE)
    typename result<stateful_visitor(X const&)>::type
  #else
    result_type
  #endif
  operator() (X const& x) const
  { return get()(x); }
  // }}}

  // {{{ binary
  template <typename X, typename Y>
  #if !defined(PRANA_NO_VISIT_REF_RESULT_TEMPLATE)
    typename result<stateful_visitor(X&, Y&)>::type
  #else
    result_type
  #endif
  operator() (X& x, Y& y) const
  { return get()(x, y); }

  template <typename X, typename Y>
  #if !defined(PRANA_NO_VISIT_REF_RESULT_TEMPLATE)
    typename result<stateful_visitor(X const&, Y&)>::type
  #else
    result_type
  #endif
  operator() (X const& x, Y& y) const
  { return get()(x, y); }
  
  template <typename X, typename Y>
  #if !defined(PRANA_NO_VISIT_REF_RESULT_TEMPLATE)
    typename result<stateful_visitor(X&, Y const&)>::type
  #else
    result_type
  #endif
  operator() (X& x, Y const& y) const
  { return get()(x, y); }

  template <typename X, typename Y>
  #if !defined(PRANA_NO_VISIT_REF_RESULT_TEMPLATE)
    typename result<stateful_visitor(X const&, Y const&)>::type
  #else
    result_type
  #endif
  operator() (X const& x, Y const& y) const
  { return get()(x, y); }
  // }}}

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

#endif // PRANA_79635E14_1E51_4747_9AE6_FAE225085715

