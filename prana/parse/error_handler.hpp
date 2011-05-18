/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_00137D0A_45CC_4269_8FF9_FF767C4751AB)
#define PRANA_00137D0A_45CC_4269_8FF9_FF767C4751AB

#include <prana/exception.hpp>
#include <prana/magic.hpp>
#include <prana/diagnostics/position_iterator.hpp>

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

  void operator() (Iterator err_pos, spirit::info const& what) const {
    throw expected_component(source, get_location(err_pos), what);
  }
};

} // prana

#endif // PRANA_00137D0A_45CC_4269_8FF9_FF767C4751AB

