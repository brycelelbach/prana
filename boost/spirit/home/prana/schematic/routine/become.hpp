/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_BECOME_HPP)
#define BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_BECOME_HPP

#include <boost/spirit/home/prana/schematic/routine/become_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

namespace routine {

//[routine_become_definition
template<class To, class From, class Dummy>
struct become {
  struct routine; 

  typedef void result_type;
  
  template<class X>
  result_type operator() (X& x) const {
    prana::clear(x);
    x.type = To::value; 
  }
};
//]

} // routine

//[become_definition
template<class To, class X>
inline void become (X& x) {
  return dispatch<typename X::registry, routine::become, To, X>(x);
}
//]

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_BECOME_HPP

