/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_SCHEMATIC_ROUTINE_SET_HPP)
#define BSP_SCHEMATIC_ROUTINE_SET_HPP

#include <boost/spirit/home/prana/schematic/routine/set_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

namespace routine {

//[routine_set_definition
template<class TagX, class TagY, class Dummy>
struct set {
  struct routine; 

  template<class> struct result;

  template<class This, class X>
  struct result<This(X&, X const&)> {
    typedef X& type;
  };
  
  template<class X>
  X& operator() (X& x, X const& y) const {
    prana::become<TagY>(x); /*< Implicit clear. >*/
    *cast<TagY>(x) = *cast<TagY>(y); /*< Assign x.data to y.data. >*/  
    return x;
  }
};
//]

//[routine_set_cons_heap_specialization
template<class TagX, class TagY>
struct set<
  TagX, TagY, typename enable_if<
    mpl::and_<
      traits::is_heap_allocated_type<TagY>,
      traits::is_cons_type<TagY>
    > 
  >::type
> {
  struct routine; 

  template<class> struct result;

  template<class This, class X>
  struct result<This(X&, X const&)> {
    typedef X& type;
  };
  
  template<class X>
  X& operator() (X& x, X const& y) const {
    prana::become<TagY>(x);

    typename TagY::data_type* xx = cast<TagY>(x);
    typename TagY::data_type const* yy = cast<TagY>(y);

    if (value_at<0>(*yy)) {
      typedef typename remove_pointer<
        typename type_at<0, typename TagY::data_type>::type
      >::type data;
      
      value_at<0>(*xx) = new data;
      
      prana::set(*value_at<0>(*xx), *value_at<0>(*yy)); 
    }

    if (value_at<1>(*yy)) {
      typedef typename remove_pointer<
        typename type_at<1, typename TagY::data_type>::type
      >::type data;

      value_at<1>(*xx) = new data;
       
      prana::set(*value_at<1>(*xx), *value_at<1>(*yy)); 
    }

    return x;
  }
};
//]

//[routine_set_vector_heap_specialization
template<class TagX, class TagY>
struct set<
  TagX, TagY, typename enable_if<
    mpl::and_<
      traits::is_heap_allocated_type<TagY>,
      traits::is_dynamic_array_type<TagY>
    > 
  >::type
> {
  struct routine; 

  template<class> struct result;

  template<class This, class X>
  struct result<This(X&, X const&)> {
    typedef X& type;
  };
  
  template<class X>
  X& operator() (X& x, X const& y) const {
    prana::become<TagY>(x);

    typename TagY::data_type* xx = cast<TagY>(x);
    typename TagY::data_type const* yy = cast<TagY>(y);

    value_at<0>(*xx) = value_at<0>(*yy); /*< TODO: Replace this value_at with
                                             size. >*/
    
    value_at<1>(*xx) = value_at<1>(*yy); /*< TODO: Replace this value_at with
                                             capacity. >*/

    typedef typename remove_pointer<
      typename type_at<2, typename TagY::data_type>::type
    >::type data;

    value_at<2>(*xx) = new data;
      
    if (value_at<0>(*xx)) 
      std::copy(
        value_at<2>(*yy),
        next(value_at<2>(*yy), value_at<0>(*xx)),
        value_at<2>(*xx)
      );

    return x; 
  }
};
//]

} // routine

//[set_visitable_definition
template<class X, class Y>
inline typename enable_if<
  traits::is_visitable<Y>,
X&>::type set (X& x, Y const& y) {
  return dispatch<typename X::registry, routine::set, X, Y const>(x, y);
}
//]

//[set_boolean_definition
template<class X>
inline X& set (X& x, bool y) {
  become<schematic::boolean>(x);
  *cast<schematic::boolean>(x) = y;
  return x;
}

template<class X>
inline X& set (X& x, mpl::true_) {
  become<schematic::boolean>(x);
  *cast<schematic::boolean>(x) = true;
  return x;
}

template<class X>
inline X& set (X& x, mpl::false_) {
  become<schematic::boolean>(x);
  *cast<schematic::boolean>(x) = false;
  return x;
}
//]

//[set_ascii_definition
template<class X>
inline X& set (X& x, char y) {
  become<schematic::ascii>(x);
  schematic::ascii::data_type* xx = cast<schematic::ascii>(x);

  value_at<0>(*xx) = 1; /*< TODO: Replace this value_at with size. >*/
  value_at<1>(*xx) = 2; /*< TODO: Replace this value_at with capacity. >*/
  value_at<2>(*xx) = new char[2];
  value_at<2>(*xx)[0] = y;

  return x;
}

template<class X>
inline X& set (X& x, char const* y) {
  become<schematic::ascii>(x);
  schematic::ascii::data_type* xx = cast<schematic::ascii>(x);

  uinthalf_t s = std::strlen(y); 

  value_at<0>(*xx) = s; /*< TODO: Replace this value_at with size. >*/

  uinthalf_t c = 2;
  while (c <= s) { c *= 2; } /*< Compute the initial capacity. >*/

  value_at<1>(*xx) = c; /*< TODO: Replace this value_at with capacity. >*/
  value_at<2>(*xx) = new char[c];

  if (value_at<0>(*xx)) 
    std::copy(y, next(y, s), value_at<2>(*xx));

  return x;
}

template<class X>
inline X& set (X& x, std::basic_string<char> const& y) {
  become<schematic::ascii>(x);
  schematic::ascii::data_type* xx = cast<schematic::ascii>(x);

  value_at<0>(*xx) = y.size(); /*< TODO: Replace this value_at with size. >*/

  uinthalf_t c = 2;
  while (c <= y.size()) { c *= 2; } 

  value_at<1>(*xx) = c; /*< TODO: Replace this value_at with capacity. >*/
  value_at<2>(*xx) = new char[c];

  if (value_at<0>(*xx)) 
    std::copy(y.begin(), y.end(), value_at<2>(*xx));

  return x;
}

template<class X, class Iterator>
inline X& set (X& x, iterator_range<Iterator> const& y) {
  become<schematic::ascii>(x);
  schematic::ascii::data_type* xx = cast<schematic::ascii>(x);

  value_at<0>(*xx) = std::distance(y.begin(), y.end()); /*< TODO: Replace this
                                                                  value_at with
                                                                  size. >*/

  uinthalf_t c = 2;
  while (c <= value_at<0>(*xx)) { c *= 2; } 

  value_at<1>(*xx) = c; /*< TODO: Replace this value_at with capacity. >*/
  value_at<2>(*xx) = new char[c];

  if (value_at<0>(*xx)) 
    std::copy(y.begin(), y.end(), value_at<2>(*xx));

  return x;
}
//]

//[set_sentinel_type_definition
template<class X>
inline X& set (X& x, sentinel_type const&) {
  become<schematic::sentinel>(x);
  return x;
}

template<class X>
inline X& set (X& x, spirit::unused_type const&) {
  become<schematic::sentinel>(x);
  return x;
}

template<class X>
inline X& set (X& x, prana::unused_type const&) {
  become<schematic::sentinel>(x);
  return x;
}
//]

//[set_integer_definition
template<class X, class Y>
inline typename enable_if<
  mpl::and_<
    is_integral<Y>,
    mpl::not_<
      is_same<Y, bool>
    >,
    mpl::not_<
      is_same<Y, char>
    >
  >,
X&>::type set (X& x, Y const& y) {
  become<schematic::integer>(x);
  *cast<schematic::integer>(x) = y;
  return x;
}
//]

//[set_floating_definition
template<class X, class Y>
inline typename enable_if<
  is_floating_point<Y>,
X&>::type set (X& x, Y const& y) {
  become<schematic::floating>(x);
  *cast<schematic::floating>(x) = y;
  return x;
}
//]

} // prana
} // spirit
} // boost

#endif // BSP_SCHEMATIC_ROUTINE_SET_HPP

