////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/filesystem/operations.hpp>

#include <boost/spirit/home/prana/phxpr/filesystem/file_predicate.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

using boost::filesystem::path;
using boost::filesystem::status;
using boost::filesystem::is_regular_file;

utree file_predicate_function::eval (utree const& subject) const {
  // TODO: exception handling.
  utf8_string_range_type range = subject.get<utf8_string_range_type>();
  path p(range.begin(), range.end());
  return utree(is_regular_file(status(p)));
}

file_predicate_composite const is_file = file_predicate_composite();

} // phxpr
} // prana
} // spirit
} // boost
