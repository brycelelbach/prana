/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCHEMATIC_CONS_HPP)
#define BOOST_SPIRIT_PRANA_SCHEMATIC_CONS_HPP

#include <boost/spirit/home/prana/schematic/core_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[cons_definition
template<class Car>
inline tuple2<schematic*, schematic*> cons (
  Car car, tuple2<schematic*, schematic*> const& cdr
) {
  schematic* p = new schematic(car, value_at<0>(cdr));
  return make_tuple2<schematic*, schematic*>(p, value_at<1>(cdr));
}

template<class Car, class Cdr>
inline tuple2<schematic*, schematic*> cons (Car car, Cdr cdr) {
  schematic* p = new schematic(car, cdr);
  return make_tuple2<schematic*, schematic*>(p, p);
}
//]

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCHEMATIC_CONS_HPP

