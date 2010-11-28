/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_FUNCTION_NODE_HPP)
#define BOOST_SPIRIT_PRANA_FUNCTION_NODE_HPP

#include <boost/spirit/home/prana/adt/scope.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class AST>
struct function_node {
  virtual ~function_node (void) { };
  virtual AST operator() (scope<AST> const&) const = 0;
  virtual function_node<AST>* clone (void) const = 0;
};

template<class AST, class F>
class stored_function: public function_node<AST> {
 private:
  F f;

 public:
  stored_function (F f = F());
  virtual ~stored_function (void) { };
  virtual AST operator() (scope<AST> const&) const;
  virtual function_node<AST>* clone (void) const;
};

template<class AST, class F>
stored_function<AST, F>::stored_function (F f): f(f) { }

template<class AST, class F>
AST stored_function<AST, F>::operator() (scope<AST> const& env) const {
  return f(env);
}

template<class AST, class F>
function_node<AST>* stored_function<AST, F>::clone (void) const {
  return new stored_function(*this);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_FUNCTION_NODE_HPP
