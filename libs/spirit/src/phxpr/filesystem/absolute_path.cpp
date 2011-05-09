////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/filesystem/operations.hpp>

#include <boost/spirit/home/prana/phxpr/filesystem/absolute_path.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

using boost::filesystem::path;
using boost::filesystem::absolute;

utree absolute_path_function::eval (utree const& subject) const {
  // TODO: exception handling.
  utf8_string_range_type range = subject.get<utf8_string_range_type>();
  path p(range.begin(), range.end());
  path r = absolute(p);
  return utree(r.native());
}

absolute_path_composite const abspath = absolute_path_composite();

} // phxpr
} // prana
} // spirit
} // boost

