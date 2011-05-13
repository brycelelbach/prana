/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_9835A676_570F_40DE_A3B3_C62EEB1DCF36)
#define BSP_9835A676_570F_40DE_A3B3_C62EEB1DCF36

#include <ostream>

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {

std::ostream& operator<< (std::ostream& out, scope const& env) {
  out << "level[" << env.level() << "]" << std::endl;

  for (scope::size_type i = 0; i < env.size(); ++i) {
    out << "  element[" << i << "]: " << env[i] << std::endl;
  }

  if (env.outer())
    out << *env.outer();

  return out;
}

} // spirit
} // boost

#endif // BSP_9835A676_570F_40DE_A3B3_C62EEB1DCF36

