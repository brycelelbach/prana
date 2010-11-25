/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_REGISTRY_HPP)
#define BOOST_SPIRIT_PRANA_REGISTRY_HPP

#include <boost/config.hpp>
#include <boost/assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/mpl/set.hpp>
#include <boost/mpl/order.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/if.hpp>

#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>

#include <boost/spirit/home/prana/traits.hpp>

#define BOOST_SPIRIT_PRANA_DECLARE_TYPE(R, _, Element)                        \
  struct BOOST_PP_SEQ_ELEM(0, Element);                                       \
  /***/

#define BOOST_SPIRIT_PRANA_DEFINE_TYPE(R, Data, Element)                      \
  struct BOOST_PP_SEQ_ELEM(0, Element) {                                      \
    struct type_definition;                                                   \
    typedef BOOST_PP_SEQ_ELEM(0, Data) value_type;                            \
    BOOST_STATIC_CONSTANT(value_type, value = mpl::order<                     \
      BOOST_PP_SEQ_ELEM(1, Data), BOOST_PP_SEQ_ELEM(0, Element)               \
    >::type::value);                                                          \
    typedef BOOST_PP_SEQ_ELEM(1, Element) type;                               \
  };                                                                          \
  /***/

#define BOOST_SPIRIT_PRANA_TYPES(ValueType, RegistrySet, Tags)                \
  BOOST_PP_SEQ_FOR_EACH(                                                      \
    BOOST_SPIRIT_PRANA_DECLARE_TYPE, _, Tags)                                 \
  typedef boost::mpl::set<BOOST_PP_SEQ_ENUM(Tags)> RegistrySet;               \
  BOOST_PP_SEQ_FOR_EACH(                                                      \
    BOOST_SPIRIT_PRANA_DEFINE_TYPE, (ValueType) (RegistrySet), Tags)          \
  /***/

namespace boost {
namespace spirit {
namespace prana {

template<typename RegistrySet>
struct basic_registry: RegistrySet {
  template<class F>
  struct which {
    typedef std::size_t result_type;

    typename enable_if<is_tag_binder<F>, std::size_t>::type
    operator() (F const& f) const {
      return (*this)(f.template get<0>());
    }
   
    template<class X> 
    typename enable_if<is_type_definition<X>, std::size_t>::type
    operator() (X const& x) const {
      return x.type;
    }
  };

  template<class F>
  struct default_ {
    typedef F& result_type;

    F& operator() (F const& f) const {
      BOOST_ASSERT(!"(error (\"invalid type information\"))");
      return f;
    };
  };

  template<class TagX>
  struct fallthrough: mpl::bool_<false> { }; 
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_REGISTRY_HPP
