////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_EAE77715_1066_46A2_9594_9C227E22E0CC)
#define BSP_EAE77715_1066_46A2_9594_9C227E22E0CC

#include <boost/spirit/home/prana/phxpr/ports/io_context.hpp>
#include <boost/spirit/home/prana/phxpr/core/composite.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct close_function: actor<close_function> {
  boost::shared_ptr<actor_list> elements;
  boost::shared_ptr<io_context> context;

  close_function (boost::shared_ptr<actor_list> const& elements_,
                  boost::shared_ptr<io_context> const& context_):
    elements(elements_), context(context_)
  {
    BOOST_ASSERT(context);
    BOOST_ASSERT(elements);
    BOOST_ASSERT(elements->size() == 1);
    BOOST_FOREACH(function const& element, *elements) {
      BOOST_ASSERT(!element.empty());
    }
  }

  utree eval (scope const& env) const {
    // TODO: typecheck (*elements)[0](env) and (*elements)[1](env)
    try {
      io_context::descriptor_type fd
        = static_cast<io_context::descriptor_type>
          ((*elements)[0](env).get<unsigned>());
      
      // TODO: replace with exception (invalid port)
      BOOST_ASSERT(fd < context->ports.size());

      // TODO: replace with exception (can't close stdin)
      BOOST_ASSERT(fd != context->stdin_port);

      // TODO: replace with exception (can't close stdout)
      BOOST_ASSERT(fd < context->stdout_port);

      // TODO: replace with exception (can't close stderr)
      BOOST_ASSERT(fd < context->stderr_port);

      // TODO: replace with exception
      BOOST_ASSERT(context->ports[fd]);
      
      context->ports[fd]->close();
      context->ports[fd].reset();

      return utree();
    } catch (...) {
      // TODO: replace with exception handling
      BOOST_ASSERT(false);
      return utree();
    }
  }
};

struct close_composite: composite<close_composite> {
  boost::shared_ptr<io_context> context;

  close_composite (boost::shared_ptr<io_context> const& context_):
    context(context_) { }

  function compose (boost::shared_ptr<actor_list> const& elements) const {
    return function(close_function(elements, context));
  }
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_EAE77715_1066_46A2_9594_9C227E22E0CC

