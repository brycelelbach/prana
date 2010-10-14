/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_POINTERS_HPP)
#define BOOST_SPIRIT_PRANA_POINTERS_HPP

#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/type_traits/remove_pointer.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/detail/sp_typeinfo.hpp>

#include <boost/spirit/home/prana/exceptions.hpp>

namespace boost {
namespace spirit {
namespace prana {

/*=============================================================================
A POD implementation of a Boost.Any like structure which unfortunately still
uses typeinfo (FIXME).
=============================================================================*/

struct any_ptr {
  template<typename Ptr>
  typename boost::disable_if<
    boost::is_polymorphic<typename boost::remove_pointer<Ptr>::type>, Ptr
  >::type get (void) const {
    if (*i == BOOST_SP_TYPEID(Ptr)) 
      return static_cast<Ptr>(p);
    
    throw bad_cast();
  }

  template<typename T>
  void construct (T* p) {
    p = p;
    i = &BOOST_SP_TYPEID(T*);
  }

  void construct (void* p, std::type_info const* i) {
    p = p;
    i = i;
  }

  void* p;
  std::type_info const* i;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_POINTERS_HPP
