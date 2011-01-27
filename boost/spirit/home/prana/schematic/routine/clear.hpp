/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_SCHEMATIC_ROUTINE_CLEAR_HPP)
#define BSP_SCHEMATIC_ROUTINE_CLEAR_HPP

#include <boost/spirit/home/prana/schematic/routine/clear_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

namespace routine {

//[routine_clear_definition
template<class TagX, class Dummy>
struct clear {
  struct routine; /*<- This incomplete type is a has_xxx detection member. ->*/

  typedef void result_type; /*<- The result_type typedef provides result_of
                                 support. ->*/
  
  template<class X>
  result_type operator() (X& x) const {
    x.type = X::sentinel::value;
    x.data[0] = 0;
    x.data[1] = 0;
  }
};
//]

//[routine_clear_cons_heap_specialization
template<class TagX>
struct clear<
  TagX, typename enable_if<
    mpl::and_<
      magic::is_heap_allocated_type<TagX>, /*<- is_heap_allocated_type is an MPL
                                         metafunction predicate that returns
                                         true if it's subject is a member of a
                                         type definition category which is
                                         allocated on the heap. ->*/
      magic::is_cons_type<TagX> /*< is_cons_type is an MPL metafunction predicate
                             which uses has_xxx support to detect cons tags. >*/
    >
  >::type
> {
  struct routine;

  typedef void result_type;

  template<class X>
  result_type operator() (X& x) const {
    typename TagX::data_type c = *cast<TagX>(x); // cast transforms the data 
                                                 // member of the supplied
                                                 // schematic into the data_type
                                                 // of the provided type
                                                 // definition, in this case,
                                                 // a prana::tuple2. 

    typename type_at<0, typename TagX::data_type>::type car;

    if ((car = value_at<0>(c)))
      delete car; /*< Implicit call to clear. >*/
    
    typename type_at<1, typename TagX::data_type>::type cdr;

    if ((cdr = value_at<1>(c)))
      delete cdr;

    x.type = X::sentinel::value;
    x.data[0] = 0;
    x.data[1] = 0;
  }
};
//]

//[routine_clear_vector_heap_specialization
template<class TagX>
struct clear<
  TagX, typename enable_if<
    mpl::and_<
      magic::is_heap_allocated_type<TagX>, 
      magic::is_dynamic_array_type<TagX> 
    >
  >::type
> {
  struct routine;

  typedef void result_type;

  template<class X>
  result_type operator() (X& x) const {
    typename TagX::data_type v = *cast<TagX>(x);

    typename type_at<2, typename TagX::data_type>::type p;

    if ((p = value_at<2>(v)))
      delete[] p;
    
    x.type = X::sentinel::value;
    x.data[0] = 0;
    x.data[1] = 0;
  }
};
//]

} // routine

//[clear_definition
template<class X>
inline void clear (X& x) {
  return dispatch<typename X::registry, routine::clear, X>(x);
}
//]

} // prana
} // spirit
} // boost

#endif // BSP_SCHEMATIC_ROUTINE_CLEAR_HPP

