////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_D874E3F5_2049_44C9_841F_A818F95B7FDD)
#define BSP_D874E3F5_2049_44C9_841F_A818F95B7FDD

#include <boost/spirit/home/prana/phxpr/ports/io_context.hpp>
#include <boost/spirit/home/prana/phxpr/core/binary_function.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct write_function: binary_function<write_function> {
  boost::shared_ptr<io_context> context;

  write_function (boost::shared_ptr<actor_list> const& elements_,
                  boost::shared_ptr<io_context> const& context_):
    base_type(elements_), context(context_)
  { BOOST_ASSERT(context); }

  typedef utree result_type;

  result_type eval (utree const& obj, utree const& port) const;
};

struct write_composite: composite<write_composite> {
  boost::shared_ptr<io_context> context;

  write_composite (boost::shared_ptr<io_context> const& context_):
    context(context_) { }

  function compose (boost::shared_ptr<actor_list> const& elements) const {
    return function(write_function(elements, context));
  }
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_D874E3F5_2049_44C9_841F_A818F95B7FDD

////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_D874E3F5_2049_44C9_841F_A818F95B7FDD)
#define BSP_D874E3F5_2049_44C9_841F_A818F95B7FDD

#include <boost/spirit/home/prana/phxpr/ports/io_context.hpp>
#include <boost/spirit/home/prana/phxpr/core/binary_function.hpp>
#include <boost/spirit/home/prana/generate/generate_sexpr.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct write_function: binary_function<write_function> {
  boost::shared_ptr<io_context> context;

  write_function (boost::shared_ptr<actor_list> const& elements_,
                  boost::shared_ptr<io_context> const& context_):
    base_type(elements_), context(context_)
  { BOOST_ASSERT(context); }

  typedef utree result_type;

  result_type eval (utree const& obj, utree const& port) const {
    // TODO: typecheck port 
    try {
      io_context::descriptor_type fd = static_cast<io_context::descriptor_type>
        (port.get<unsigned>());
     
      BOOST_ASSERT(context->ports.size() <= fd);
 
      return utree(generate_sexpr(obj, *context->ports[fd]));
    } catch (...) {
      // TODO: replace with exception handling
      BOOST_ASSERT(false);
      return utree();
    }
  }
};

struct write_composite: composite<write_composite> {
  boost::shared_ptr<io_context> context;

  write_composite (boost::shared_ptr<io_context> const& context_):
    context(context_) { }

  function compose (boost::shared_ptr<actor_list> const& elements) const {
    return function(write_function(elements, context));
  }
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_D874E3F5_2049_44C9_841F_A818F95B7FDD

