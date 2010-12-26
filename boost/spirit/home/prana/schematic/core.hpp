/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCHEMATIC_CORE_HPP)
#define BOOST_SPIRIT_PRANA_SCHEMATIC_CORE_HPP

#include <boost/spirit/home/prana/schematic/core_fwd.hpp>
#include <boost/spirit/home/prana/schematic/routine/clear.hpp>
#include <boost/spirit/home/prana/schematic/routine/become.hpp>
#include <boost/spirit/home/prana/schematic/routine/set.hpp>
#include <boost/spirit/home/prana/schematic/routine/set_car.hpp>
#include <boost/spirit/home/prana/schematic/routine/set_cdr.hpp>

namespace boost {
namespace spirit {
namespace prana {

schematic::schematic (void): type(sentinel::value) {
  data[0] = 0;
  data[1] = 0;
}

schematic::~schematic (void) {
  clear(*this);
}

template<class Value>
schematic::schematic (Value const& val): type(sentinel::value) {
  data[0] = 0;
  data[1] = 0;
  set(*this, val);
}

template<class Value>
schematic& schematic::operator= (Value const& val) {
  // TODO: Add eqv test here.
  set(*this, val);
}

template<class Value>
void schematic::assign (Value const& val) {
  // TODO: Add eqv test here.
  set(*this, val);
}

schematic::schematic (tuple2<schematic*, schematic*> const& range):
  type(sentinel::value)
{
  data[0] = 0;
  data[1] = 0;
  assign(range);
}

void schematic::assign (tuple2<schematic*, schematic*> const& range) {
  become<list>(*this);
  set_car(*this, value_at<0>(range));
  set_cdr(*this, value_at<1>(range)); 
}

template<class Car, class Cdr>
schematic::schematic (Car car, Cdr cdr): type(sentinel::value) {
  data[0] = 0;
  data[1] = 0;
  assign(car, cdr);
}

template<class Car, class Cdr>
void schematic::assign (Car car, Cdr cdr) {
  become<pair>(*this);
  set_car(*this, car);
  set_cdr(*this, cdr); 
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCHEMATIC_CORE_HPP

