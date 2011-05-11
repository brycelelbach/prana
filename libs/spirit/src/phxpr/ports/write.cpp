////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/spirit/home/prana/phxpr/ports/write.hpp>
#include <boost/spirit/home/prana/generate/generate_sexpr.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

write_function::result_type
write_function::eval (utree const& obj, utree const& port) const {
  // TODO: typecheck port 
  try {
    io_context::descriptor_type fd = static_cast<io_context::descriptor_type>
      (port.get<unsigned>());
   
    BOOST_ASSERT(context->ports.size() <= fd);

    return utree(generate_sexpr(obj, *context->ports[fd]));
  } catch (...) {
    // TODO: replace with exception handling
    BOOST_ASSERT(false);
    return utree(false);
  }
}

} // phxpr
} // prana
} // spirit
} // boost

