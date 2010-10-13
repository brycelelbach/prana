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

// forward declaration
class utree;

// forward declaration
template<typename UTreeX, typename UTreeY>
struct visitor;

struct void_ptr {
  void* p;
  std::type_info const* i;
};

class any_ptr {
 public:
  template<typename Ptr>
  typename boost::disable_if<
    boost::is_polymorphic<typename boost::remove_pointer<Ptr>::type>, Ptr
  >::type get (void) const {
    if (*i == BOOST_SP_TYPEID(Ptr)) 
      return static_cast<Ptr>(p);
    
    throw bad_cast();
  }

  template<typename T>
  any_ptr (T* p): p(p), i(&BOOST_SP_TYPEID(T*)) { }

  friend bool operator== (any_ptr const& a, any_ptr const& b) {
    return (a.p == b.p) && (*a.i == *b.i);
  }

 private:
  any_ptr (void* p, std::type_info const* i): p(p), i(i) { }

  template<typename UTreeX, typename UTreeY>
  friend struct vistor;

  friend class utree;

  void* p;
  std::type_info const* i;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_POINTERS_HPP