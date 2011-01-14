/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TRAITS_HPP)
#define BOOST_SPIRIT_PRANA_TRAITS_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/set.hpp>

#include <boost/spirit/home/prana/adt/source_location.hpp>
#include <boost/spirit/home/prana/adt/dynamic_array.hpp>
#include <boost/spirit/home/prana/traits_fwd.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace traits {

template<class Tag, class Enable/* = void*/>
struct annotations_type: dynamic_array<fusion::vector1<source_location> > { };  

template<class Tag, class Enable/* = void*/>
struct has_source_locations: mpl::false_ { };

template<class Tag, class Enable/* = void*/>
struct extract_source_location_from_node {
  typedef std::size_t type; 

  template<class PT>
  static type call (utree const& ut, PT const&) {
    switch (ut.which()) {
      case utree_type::string_type:
      case utree_type::symbol_type:
      case utree_type::binary_type:
        return -1;
      default:
        return ut.tag();
    }
  }
};

template<class PT>
typename prana::result_of::extract_source_location<typename PT::tag>::type
extract_source_location (utree const& ut, PT const& pt) {
  return extract_source_location_from_node<typename PT::tag>::call(ut, pt);
}

template<class Tag, class Enable/* = void*/>
struct has_list_subtypes: mpl::false_ { };

template<class Tag, class Enable/* = void*/>
struct list_subtypes: mpl::set<> { };

template<class Tag, class Enable/* = void*/>
struct extract_list_subtype_from_node {
  typedef std::size_t type; 

  template<class PT>
  static type call (utree const&, PT const&) {
    return -1;
  }
};

template<class PT>
typename prana::result_of::extract_list_subtype<typename PT::tag>::type
extract_list_subtype (utree const& ut, PT const& pt) {
  return extract_list_subtype_from_node<typename PT::tag>::call(ut, pt);
}

} // traits

namespace result_of {

template<class Tag, class Enable/* = void*/>
struct extract_source_location:
  traits::extract_source_location_from_node<Tag> { };

template<class Tag, class Enable/* = void*/>
struct extract_list_subtype:
  traits::extract_list_subtype_from_node<Tag> { };

} // result_of
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TRAITS_HPP

