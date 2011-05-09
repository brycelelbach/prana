////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_7067B9DA_391B_4D42_975F_B71326E39FB6)
#define BSP_7067B9DA_391B_4D42_975F_B71326E39FB6

#include <ios>

#include <boost/spirit/home/prana/phxpr/ports/io_context.hpp>
#include <boost/spirit/home/prana/phxpr/core/composite.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct open_function: actor<open_function> {
  boost::shared_ptr<actor_list> elements;
  boost::shared_ptr<io_context> context;

  open_function (boost::shared_ptr<actor_list> const& elements_,
                       boost::shared_ptr<io_context> const& context_):
    elements(elements_), context(context_)
  {
    BOOST_ASSERT(context);
    BOOST_ASSERT(elements);
    BOOST_ASSERT(elements->size() == 2);
    BOOST_FOREACH(function const& element, *elements) {
      BOOST_ASSERT(!element.empty());
    }
  }

  utree eval (scope const& env) const {
    // TODO: typecheck (*elements)[0](env) and (*elements)[1](env)
    try {
      utree ut = (*elements)[0](env);

      utf8_string_range_type range = ut.get<utf8_string_range_type>();

      std::string path(range.begin(), range.end());
     
      std::ios_base::openmode mode = static_cast<std::ios_base::openmode>
        ((*elements)[1](env).get<unsigned>());
      
      boost::shared_ptr<port> p(new port(path, mode));

      context->ports.push_back(p);

      return utree(context->ports.size() - 1); 
    } catch (...) {
      // TODO: replace with exception handling
      BOOST_ASSERT(false);
      return utree();
    }
  }
};

struct open_composite: composite<open_composite> {
  boost::shared_ptr<io_context> context;

  open_composite (boost::shared_ptr<io_context> const& context_):
    context(context_) { }

  function compose (boost::shared_ptr<actor_list> const& elements) const {
    return function(open_function(elements, context));
  }
};

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_7067B9DA_391B_4D42_975F_B71326E39FB6

