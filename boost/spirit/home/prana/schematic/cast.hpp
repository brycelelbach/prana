/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCHEMATIC_CAST_CAR_HPP)
#define BOOST_SPIRIT_PRANA_SCHEMATIC_CAST_CAR_HPP

#include <boost/spirit/home/prana/domain.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[caster_declaration
template<class TagX, class Member, Member Ptr, class Dummy = void>
struct caster;
//]

//[cast_declaration
template<class TagX, class X>
inline typename TagX::data_type* cast (X& x);

template<class TagX, class X>
inline typename TagX::data_type const* cast (X const& x);
//]

//[caster_definition
template<class TagX, class Object, class Type, Type Object::*Ptr>
struct caster<TagX, Type Object::*, Ptr> {
  template<class> struct result;

  template<class This>
  struct result<This(Object&)> {
    typedef typename TagX::data_type* type;
  };

  template<class This>
  struct result<This(Object const&)> {
    typedef typename TagX::data_type const* type;
  };

  typename TagX::data_type* operator() (Object& obj) const {
    return reinterpret_cast<typename TagX::data_type*>(&(obj.*Ptr));
  }
  
  typename TagX::data_type const* operator() (Object const& obj) const {
    return reinterpret_cast<typename TagX::data_type const*>(&(obj.*Ptr));
  }
};
//]

//[cast_definition
template<class TagX, class X>
inline typename TagX::data_type* cast (X& x) {
  return caster<TagX, void* (X::*)[2], &X::data>()(x);
}

template<class TagX, class X>
inline typename TagX::data_type const* cast (X const& x) {
  return caster<TagX, void* (X::*)[2], &X::data>()(x);
}
//]

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCHEMATIC_CAST_CAR_HPP

