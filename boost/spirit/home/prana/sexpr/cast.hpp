/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_CAST_CAR_HPP)
#define BOOST_SPIRIT_PRANA_CAST_CAR_HPP

#include <boost/type_traits/is_pointer.hpp>

#include <boost/spirit/home/prana/domain.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class TagX, class Member, Member Ptr, class Dummy = prana::unused_type>
struct caster;

template<class TagX, class Object, class Type, Type Object::*Ptr>
struct caster<TagX, Type Object::*, Ptr, prana::unused_type> {
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

template<class TagX, class X>
inline typename TagX::data_type* cast_car (X& x) {
  return caster<TagX, void* X::*, &X::car>()(x);
}

template<class TagX, class X>
inline typename TagX::data_type const* cast_car (X const& x) {
  return caster<TagX, void* X::*, &X::car>()(x);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_CAST_CAR_HPP

