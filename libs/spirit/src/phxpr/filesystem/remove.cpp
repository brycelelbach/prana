////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include <boost/spirit/home/prana/phxpr/filesystem/remove.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

utree remove_function::eval (utree const& subject) const {
  utf8_string_range_type path = subject.get<utf8_string_range_type>();
  return utree(::remove(path.begin()));
}

remove_composite const remove_ = remove_composite();

} // phxpr
} // prana
} // spirit
} // boost

