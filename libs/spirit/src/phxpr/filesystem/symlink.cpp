////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/filesystem/operations.hpp>

#include <boost/spirit/home/prana/phxpr/filesystem/symlink.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

using boost::filesystem::path;
using boost::filesystem::create_symlink;

utree symlink_function::eval (utree const& source, utree const& dest) const {
  // TODO: exception handling.
  utf8_string_range_type source_range = source.get<utf8_string_range_type>();
  path source_path(source_range.begin(), source_range.end());

  utf8_string_range_type dest_range = dest.get<utf8_string_range_type>();
  path dest_path(dest_range.begin(), dest_range.end());

  try {
    create_symlink(source_path, dest_path);
    return utree(true);
  }
 
  catch (...) { return utree(false); }
}

symlink_composite const symlink_ = symlink_composite();

} // phxpr
} // prana
} // spirit
} // boost

