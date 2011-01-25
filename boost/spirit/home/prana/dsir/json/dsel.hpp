/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_DSIR_JSON_DSEL_HPP)
#define BSP_DSIR_JSON_DSEL_HPP

#include <boost/proto/core.hpp>
#include <boost/proto/transform.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include <boost/type_traits/add_reference.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class T>
struct singleton {
  static T const data;
};

template<class T>
T const singleton<T>::data = utree();

// callable function object
struct json_access_find: proto::callable {
  template<class> struct result;

  template<class This, class IR, class Key>
  struct result<This(IR const&, Key&)> {
    typedef IR const& type;
  };

  template<class IR, class Key>
  IR const& operator() (IR const& ir, Key& key) const {
    if (!ir.count(key))
      return singleton<IR>::data;
    else
      return ir.find(key);    
  }
};

// grammar 
struct json_access_grammar:
  proto::or_<
    proto::when<
      proto::subscript<
        proto::terminal<proto::_>,
        proto::terminal<proto::_>
      >,
      json_access_find(
        proto::_value(proto::_child0),
        proto::_value(proto::_child1)
      )
    >,
    proto::when<
      proto::subscript<
        json_access_grammar,
        proto::terminal<proto::_>
      >,
      json_access_find(
        json_access_grammar(proto::_child0),
        proto::_value(proto::_child1)
      )
    >,
    proto::terminal<proto::_>
  > { };

template<class Expr>
struct json_access_expr;

// domain
struct json_access_domain:
  proto::domain<
    proto::pod_generator<json_access_expr>,
    json_access_grammar
  > { };

} // prana
} // spirit
} // boost

#endif // BSP_DSIR_JSON_DSEL_HPP

