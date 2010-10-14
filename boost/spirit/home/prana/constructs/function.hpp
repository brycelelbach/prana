/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_FUNCTION_HPP)
#define BOOST_SPIRIT_PRANA_FUNCTION_HPP

#include <boost/spirit/home/prana/constructs/record.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename F, typename Tree>
struct function: public record<Tree> {
 public:
  F f;

  function (F f_ = F()): f(f_) { }

  virtual ~function (void) { }

  virtual Tree operator() (environment<Tree> const& env) const {
    return f(env);
  }
  
  virtual record<Tree>* clone (void) const {
    return new function<F>(*this);
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_FUNCTION_HPP
