/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_2BE1B0EF_A840_49A5_B31D_83E4E3F24EA0)
#define BSP_2BE1B0EF_A840_49A5_B31D_83E4E3F24EA0

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

#include <boost/spirit/home/prana/magic.hpp>
#include <boost/spirit/home/prana/exception.hpp>
#include <boost/spirit/home/prana/support/nt2/preprocessor.hpp>

namespace boost {
namespace spirit {
namespace prana {

#define BSP_COLUMN(R, Column, Element)   \
  BOOST_PP_SEQ_ELEM(Column, Element)     \
  /***/

#define BSP_DECLARE_TYPE(R, _, Element)  \
  struct BOOST_PP_SEQ_ELEM(0, Element);  \
  /***/

#define BSP_DEFINE_TYPE(R, Data, Element)                           \
  struct BOOST_PP_SEQ_ELEM(0, Element) {                            \
    struct type_definition;                                         \
    struct BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(2, Element), _type);      \
    struct BOOST_PP_SEQ_ELEM(3, Element);                           \
    typedef BOOST_PP_SEQ_ELEM(0, Data) value_type;                  \
    BOOST_STATIC_CONSTANT(value_type, value = (mpl::order<          \
      BOOST_PP_SEQ_ELEM(1, Data), BOOST_PP_SEQ_ELEM(0, Element)     \
    >::type::value));                                               \
    typedef NT2_PP_STRIP(BOOST_PP_SEQ_ELEM(1, Element)) data_type;  \
  };                                                                \
  /***/

#define BSP_REGISTRY_SET(ValueType, RegistrySet, Tags)         \
  BOOST_PP_SEQ_FOR_EACH(                                       \
    BSP_DECLARE_TYPE, _, Tags)                                 \
  typedef boost::mpl::set<BOOST_PP_SEQ_ENUM(                   \
    BOOST_PP_SEQ_TRANSFORM(BSP_COLUMN, 0, Tags))> RegistrySet; \
  BOOST_PP_SEQ_FOR_EACH(                                       \
    BSP_DEFINE_TYPE, (ValueType) (RegistrySet), Tags)          \
  /***/

template<typename RegistrySet>
struct basic_registry: RegistrySet {
  struct type_registry;

  typedef RegistrySet registry_set;

  struct which {
    typedef std::size_t result_type;

    template<class F>
    typename enable_if<magic::is_tag_binder<F>, std::size_t>::type
    operator() (F const& f) const {
      return (*this)(f.template get<0>());
    }
   
    template<class Visitable> 
    typename enable_if<magic::is_visitable<Visitable>, std::size_t>::type
    operator() (Visitable const& v) const {
      return v.type;
    }
    
    template<class TagX> 
    typename enable_if<magic::is_type_definition<TagX>, std::size_t>::type
    operator() (TagX const&) const {
      return TagX::value;
    }
  };

  template<class Result>
  struct default_ {
    typedef Result result_type;

    template<class F>
    result_type operator() (F const& f) const {
      BOOST_THROW_EXCEPTION(invalid_visitable_typeinfo(which()(f)));
    };
  };
};

} // prana
} // spirit
} // boost

#endif // BSP_2BE1B0EF_A840_49A5_B31D_83E4E3F24EA0

