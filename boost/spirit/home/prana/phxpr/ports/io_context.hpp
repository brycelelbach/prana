////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_EF6E5263_C8E6_49DE_A216_20C60C7BFF04)
#define BSP_EF6E5263_C8E6_49DE_A216_20C60C7BFF04

#include <boost/shared_ptr.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>

#include <boost/spirit/home/prana/adt/dynamic_array.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

typedef boost::iostreams::stream<
  boost::iostreams::file_descriptor
> port;

struct io_context {
  typedef dynamic_array<boost::shared_ptr<port> > ports_type;
  typedef ports_type::size_type descriptor_type; 

  ports_type ports;

  descriptor_type current_input_port;
  descriptor_type current_output_port;
  descriptor_type current_error_port;

  descriptor_type stdin_port;
  descriptor_type stdout_port;
  descriptor_type stderr_port;

  io_context (void);
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_EF6E5263_C8E6_49DE_A216_20C60C7BFF04

