////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include <boost/spirit/home/prana/phxpr/ports/io_context.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

using boost::iostreams::never_close_handle;
using boost::iostreams::file_descriptor;

io_context::io_context (void):
  current_input_port(0),
  current_output_port(1),
  current_error_port(2),
  stdin_port(0),
  stdout_port(1),
  stderr_port(2)
{
  boost::shared_ptr<port> stdin_(new port
    (file_descriptor(::fileno(stdin), never_close_handle)));
  boost::shared_ptr<port> stdout_(new port
    (file_descriptor(::fileno(stdout), never_close_handle)));
  boost::shared_ptr<port> stderr_(new port
    (file_descriptor(::fileno(stderr), never_close_handle)));

  ports.push_back(stdin_); 
  ports.push_back(stdout_); 
  ports.push_back(stderr_); 
}

} // phxpr
} // prana
} // spirit
} // boost

