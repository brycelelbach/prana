/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_FUNCTION_NODE_HPP)
#define BOOST_SPIRIT_PRANA_ADT_FUNCTION_NODE_HPP

#include <boost/spirit/home/prana/adt/scope.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[function_node
template<class UniversalTree>
struct function_node {
  virtual ~function_node (void) { };
  virtual UniversalTree operator() (scope<UniversalTree> const&) const = 0;
  virtual function_node<UniversalTree>* clone (void) const = 0;
};
//]

//[stored_function
template<class UniversalTree, class F>
class stored_function: public function_node<UniversalTree> {
 private:
  F f;

 public:
  stored_function (F f = F());
  virtual ~stored_function (void) { };
  virtual UniversalTree operator() (scope<UniversalTree> const&) const;
  virtual function_node<UniversalTree>* clone (void) const;
};
//]

template<class UniversalTree, class F>
stored_function<UniversalTree, F>::stored_function (F f): f(f) { }

template<class UniversalTree, class F>
UniversalTree stored_function<UniversalTree, F>::operator() (
  scope<UniversalTree> const& env
) const {
  return f(env);
}

template<class UniversalTree, class F>
function_node<UniversalTree>*
stored_function<UniversalTree, F>::clone (void) const {
  return new stored_function(*this);
}

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_ADT_FUNCTION_NODE_HPP ->*/

