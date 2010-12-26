/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_FUNCTION_NODE_HPP)
#define BOOST_SPIRIT_PRANA_ADT_FUNCTION_NODE_HPP

#include <boost/spirit/home/prana/adt/scope.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[function_node
template<class Visitable>
struct function_node {
  virtual ~function_node (void) { };
  virtual Visitable operator() (scope<Visitable> const&) const = 0;
  virtual function_node<Visitable>* clone (void) const = 0;
};
//]

//[stored_function
template<class Visitable, class F>
class stored_function: public function_node<Visitable> {
 private:
  F f;

 public:
  stored_function (F f = F());
  virtual ~stored_function (void) { };
  virtual Visitable operator() (scope<Visitable> const&) const;
  virtual function_node<Visitable>* clone (void) const;
};
//]

template<class Visitable, class F>
stored_function<Visitable, F>::stored_function (F f): f(f) { }

template<class Visitable, class F>
Visitable stored_function<Visitable, F>::operator() (
  scope<Visitable> const& env
) const {
  return f(env);
}

template<class Visitable, class F>
function_node<Visitable>*
stored_function<Visitable, F>::clone (void) const {
  return new stored_function(*this);
}

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_ADT_FUNCTION_NODE_HPP ->*/

