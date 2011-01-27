/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_00137D0A_45CC_4269_8FF9_FF767C4751AB)
#define BSP_00137D0A_45CC_4269_8FF9_FF767C4751AB

#include <boost/spirit/home/prana/exception.hpp>
#include <boost/spirit/home/prana/magic.hpp>
#include <boost/spirit/home/prana/adt/position_iterator.hpp>

namespace boost {
namespace spirit {
namespace prana {
    
template<class Tag, class Iterator>
struct error_handler {
  typedef typename magic::source_type<Tag>::type source_type;

  template<class, class>
  struct result {
    typedef void type;
  };

  source_type const& source;

  error_handler (source_type const& source_): source(source_) { }

  void operator() (Iterator err_pos, info const& what) const {
    throw expected_component(source, get_location(err_pos), what);
  }
};

} // prana
} // spirit
} // boost

#endif // BSP_00137D0A_45CC_4269_8FF9_FF767C4751AB

