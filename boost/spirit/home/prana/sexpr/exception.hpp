/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_EXCEPTION_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_EXCEPTION_HPP

#include <boost/throw_exception.hpp>

#include <boost/spirit/home/prana/sexpr/routine/serialize.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct sexpr_exception: std::exception { };

struct expected_dynamic_array: sexpr_exception {
  std::string msg;

  template<class X>
  expected_dynamic_array (X const& x) {
    msg = "'(expected-dynamic-array " + serialize(x) + ")"; 
  }

  template<class X, class Y>
  expected_dynamic_array (X const& x, Y const& y) {
    msg = "'(expected-dynamic-array " + serialize(x) + " " + serialize(y) + ")";
  }

  virtual ~expected_dynamic_array (void) throw() { }

  virtual const char* what (void) const throw() { return msg.c_str(); }
};

struct expected_cons: sexpr_exception {
  std::string msg;

  template<class X>
  expected_cons (X const& x) {
    msg = "'(expected-cons " + serialize(x) + ")"; 
  }

  template<class X, class Y>
  expected_cons (X const& x, Y const& y) {
    msg = "'(expected-cons " + serialize(x) + " " + serialize(y) + ")";
  }

  virtual ~expected_cons (void) throw() { }

  virtual const char* what (void) const throw() { return msg.c_str(); }
};

struct expected_composite: sexpr_exception {
  std::string msg;

  template<class X>
  expected_composite (X const& x) {
    msg = "'(expected-composite " + serialize(x) + ")"; 
  }

  template<class X, class Y>
  expected_composite (X const& x, Y const& y) {
    msg = "'(expected-composite " + serialize(x) + " " + serialize(y) + ")";
  }

  virtual ~expected_composite (void) throw() { }

  virtual const char* what (void) const throw() { return msg.c_str(); }
};

struct storage_unavailable: sexpr_exception {
  std::string msg;

  template<class X>
  storage_unavailable (X const& x) {
    msg = "'(storage-unavailable " + serialize(x) + ")"; 
  }

  template<class X, class Y>
  storage_unavailable (X const& x, Y const& y) {
    msg = "'(storage-unavailable " + serialize(x) + " " + serialize(y) + ")";
  }

  virtual ~storage_unavailable (void) throw() { }

  virtual const char* what (void) const throw() { return msg.c_str(); }
};

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_EXCEPTION_HPP ->*/
