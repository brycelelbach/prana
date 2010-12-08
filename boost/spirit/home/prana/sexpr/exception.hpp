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

struct expected_dynamic_array: std::exception {
  std::string msg;

  template<class X>
  expected_dynamic_array (X const& gotx) {
    msg = "(exception '(expected-dynamic-array ";
    msg += serialize(gotx);
    msg += "))"; 
  }

  template<class X, class Y>
  expected_dynamic_array (X const& gotx, Y const& goty) {
    msg = "(exception '(expected-dynamic-array ";
    msg += serialize(gotx);
    msg += " "; 
    msg += serialize(goty);
    msg += "))"; 
  }

  virtual ~expected_dynamic_array (void) throw() { }

  virtual const char* what (void) const throw() { return msg.c_str(); }
};

struct expected_cons: std::exception {
  std::string msg;

  template<class X>
  expected_cons (X const& gotx) {
    msg = "(exception '(expected-cons ";
    msg += serialize(gotx);
    msg += "))"; 
  }

  template<class X, class Y>
  expected_cons (X const& gotx, Y const& goty) {
    msg = "(exception '(expected-cons ";
    msg += serialize(gotx);
    msg += " "; 
    msg += serialize(goty);
    msg += "))"; 
  }

  virtual ~expected_cons (void) throw() { }

  virtual const char* what (void) const throw() { return msg.c_str(); }
};

struct storage_unavailable: std::exception {
  std::string msg;

  template<class X>
  storage_unavailable (X const& gotx) {
    msg = "(exception '(storage-unavailable ";
    msg += serialize(gotx);
    msg += "))"; 
  }

  template<class X, class Y>
  storage_unavailable (X const& gotx, Y const& goty) {
    msg = "(exception '(storage-unavailable ";
    msg += serialize(gotx);
    msg += " "; 
    msg += serialize(goty);
    msg += "))"; 
  }

  virtual ~storage_unavailable (void) throw() { }

  virtual const char* what (void) const throw() { return msg.c_str(); }
};

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_EXCEPTION_HPP ->*/
