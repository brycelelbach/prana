/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_9835A676_570F_40DE_A3B3_C62EEB1DCF36)
#define PRANA_9835A676_570F_40DE_A3B3_C62EEB1DCF36

#include <prana/config.hpp>

#include <ostream>

#include <boost/spirit/include/support_utree.hpp>

#include <prana/generate/generate_sexpr.hpp>

// TODO: move as much of this as possible into a source file.

namespace boost {
namespace spirit {

inline std::ostream& operator<< (std::ostream& out, scope const& env)
{
  out << "level[" << env.level() << "]" << std::endl;

  for (prana::scope::size_type i = 0; i < env.size(); ++i) {
    out << "  element[" << i << "]: " << env[i] << std::endl;
  }

  if (env.outer())
    out << *env.outer();

  return out;
}

inline std::ostream& operator<< (std::ostream& out, utree const& ut)
{
  prana::generate_sexpr(ut, out);
  return out; 
}

} // spirit
} // boost

#endif // PRANA_9835A676_570F_40DE_A3B3_C62EEB1DCF36

