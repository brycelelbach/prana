/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_DSIR_JSON_DSEL_HPP)
#define BOOST_SPIRIT_PRANA_DSIR_JSON_DSEL_HPP

#include <boost/proto/core.hpp>
#include <boost/proto/transform.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include <boost/type_traits/add_reference.hpp>

namespace boost {
namespace proto {

// crtp_generator
template<template<class, class> class Extends, class Derived>
struct crtp_generator {
  BOOST_PROTO_CALLABLE()
  BOOST_PROTO_USE_BASIC_EXPR()

  template<class> struct result;

  template<class This, class Expr>
  struct result<This(Expr)> {
    typedef Extends<Expr, Derived> type;
  };

  template<class This, class Expr>
  struct result<This(Expr &)> {
    typedef Extends<Expr, Derived> type;
  };

  template<class This, class Expr>
  struct result<This(Expr const &)> {
    typedef Extends<Expr, Derived> type;
  };

  template<class Expr>
  Extends<Expr, Derived> operator() (Expr const &e) const {
    return Extends<Expr, Derived>(e);
  }
};

} // proto

namespace spirit {
namespace prana {
namespace tag {

struct json_root { };

} // tag

// callable function object
template<class Derived>
struct json_access_find: proto::callable {
  static Derived const sentinel;

  template<class> struct result;

  template<class This, class Key>
  struct result<This(Derived const&, Key const&)> {
    typedef Derived const& type;
  };

  template<class Key>
  Derived const& operator() (Derived const& ir, Key const& key) const {
    if (!ir.count(key))
      return sentinel;
    else
      return ir.find(key);    
  }
};

template<class Derived>
Derived const json_access_find<Derived>::sentinel = utree();

// grammar 
template<class Derived>
struct json_access_grammar:
  proto::or_<
    proto::when<
      proto::divides<
        proto::terminal<tag::json_root>,
        proto::terminal<proto::_>
      >,
      json_access_find<Derived>(
        proto::_data,
        proto::_value(proto::_child1)
      )
    >,
    proto::when<
      proto::divides<
        json_access_grammar<Derived>,
        proto::terminal<proto::_>
      >,
      json_access_find<Derived>(
        json_access_grammar<Derived>(proto::_child0),
        proto::_value(proto::_child1)
      )
    >,
    proto::terminal<proto::_>
  > { };

template<class Expr, class Derived>
struct json_access_expr;

// domain
template<class Derived>
struct json_access_domain:
  proto::domain<
    proto::crtp_generator<json_access_expr, Derived>,
    json_access_grammar<Derived>
  > { };

// expression wrapper
template<class Expr, class Derived>
struct json_access_expr:
  proto::extends<
    Expr, json_access_expr<Expr, Derived>, json_access_domain<Derived>
  >
{
  typedef proto::extends<
    Expr, json_access_expr<Expr, Derived>, json_access_domain<Derived>
  > proto_base_type;

  json_access_expr (Expr const& expr = Expr()): proto_base_type(expr) { }

  Derived const& derived (void) const {
    return *static_cast<Derived const*>(this);
  }

  operator Derived (void) const {
    BOOST_MPL_ASSERT((proto::matches<Expr, json_access_grammar<Derived> >));
    return json_access_grammar<Derived>()(*this, 0, derived());
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_DSIR_JSON_DSEL_HPP

