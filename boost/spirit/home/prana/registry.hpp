/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_REGISTRY_HPP)
#define BOOST_SPIRIT_PRANA_REGISTRY_HPP

#include <boost/config.hpp>
#include <boost/assert.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mpl/equal.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/order.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/comparison.hpp>

#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>

#include <boost/spirit/home/prana/traits.hpp>
#include <boost/spirit/home/prana/exception.hpp>
#include <boost/spirit/home/prana/support/nt2/preprocessor.hpp>

namespace boost {
namespace spirit {
namespace prana {

#define BOOST_SPIRIT_PRANA_COLUMN(R, Column, Element)   \
  BOOST_PP_SEQ_ELEM(Column, Element)                    \
  /***/

#define BOOST_SPIRIT_PRANA_DECLARE_TYPE(R, _, Element)  \
  struct BOOST_PP_SEQ_ELEM(0, Element);                 \
  /***/

#define BOOST_SPIRIT_PRANA_DEFINE_TYPE(R, Data, Element)                      \
  struct BOOST_PP_SEQ_ELEM(0, Element) {                                      \
    struct type_definition;                                                   \
    struct BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(2, Element), _type);                \
    struct BOOST_PP_SEQ_ELEM(3, Element);                                     \
    typedef BOOST_PP_SEQ_ELEM(0, Data) value_type;                            \
    BOOST_STATIC_CONSTANT(value_type, value = (mpl::order<                    \
      BOOST_PP_SEQ_ELEM(1, Data), BOOST_PP_SEQ_ELEM(0, Element)               \
    >::type::value));                                                         \
    typedef NT2_PP_STRIP(BOOST_PP_SEQ_ELEM(1, Element)) data_type;            \
  };                                                                          \
  /***/

#define BOOST_SPIRIT_PRANA_REGISTRY_SET(ValueType, RegistrySet, Tags)         \
  BOOST_PP_SEQ_FOR_EACH(                                                      \
    BOOST_SPIRIT_PRANA_DECLARE_TYPE, _, Tags)                                 \
  typedef boost::mpl::set<BOOST_PP_SEQ_ENUM(                                  \
    BOOST_PP_SEQ_TRANSFORM(BOOST_SPIRIT_PRANA_COLUMN, 0, Tags))> RegistrySet; \
  BOOST_PP_SEQ_FOR_EACH(                                                      \
    BOOST_SPIRIT_PRANA_DEFINE_TYPE, (ValueType) (RegistrySet), Tags)          \
  /***/

//[basic_registry
template<typename RegistrySet>
struct basic_registry: RegistrySet {
  typedef RegistrySet registry_set;

  struct which {
    typedef std::size_t result_type;

    template<class F>
    typename enable_if<
      is_tag_binder<F>,
      std::size_t
    >::type operator() (F const& f) const {
      return (*this)(f.template get<0>());
    }
   
    template<class UniversalTree> 
    typename enable_if<
      is_universal_tree<UniversalTree>,
      std::size_t
    >::type operator() (UniversalTree const& t) const {
      return t.type;
    }
    
    template<class TagX> 
    typename enable_if<
      is_type_definition<TagX>,
      std::size_t
    >::type operator() (TagX const&) const {
      return TagX::value;
    }
  };

  template<class Result>
  struct default_ {
    typedef Result result_type;

    template<class F>
    result_type operator() (F const& f) const {
      BOOST_THROW_EXCEPTION(invalid_type_information(which()(f)));
    };
  };
};
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_REGISTRY_HPP ->*/

