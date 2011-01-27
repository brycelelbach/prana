/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_18598222_9F84_4C7B_9533_ABE2065634D4)
#define BSP_18598222_9F84_4C7B_9533_ABE2065634D4

#include <boost/spirit/home/prana/config.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>

#include <boost/spirit/home/prana/phxpr/core/composite.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {
    
struct this_function: actor<this_function> {
  boost::shared_ptr<actor_list> elements;
  scope::size_type level;

  typedef utree result_type;

  this_function (boost::shared_ptr<actor_list> elements_,
                 scope::size_type level_ = 0):
    elements(elements_), level(level_)
  {
    BOOST_ASSERT(elements);
  }
  
  utree eval (scope const& env) const {
    scope const* outer = &env; // Get the parent scope.

    while (level != outer->level())
      outer = outer->outer();

    if (elements && !elements->empty()) {
      boost::scoped_array<utree> fargs(new utree[elements->size() + 1]);

      std::size_t i = 1;
      fargs[0] = clone();

      BOOST_FOREACH(function const& element, *elements) {
        fargs[i++] = element(env);
      }

      utree* fi = fargs.get();

      utree const& arg = env[0];
      return arg.eval(scope(fi, fi + elements->size() + 1, outer));
    }
    
    else {
      boost::scoped_ptr<utree> this_(new utree(clone()));
      utree const& arg = env[0];
      return arg.eval(scope(this_.get(), 0, outer));
    }
  }

  utree clone (void) const {
    return utree(stored_function<this_function>(
      this_function(elements, level)));
  }
};

struct this_composite: composite<this_composite> {
  scope::size_type const level;

  this_composite (scope::size_type level_): level(level_) { }

  function compose (boost::shared_ptr<actor_list> const& elements) const {
    return function(this_function(elements, level));
  }
};

this_composite const _this = this_composite(0);

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_18598222_9F84_4C7B_9533_ABE2065634D4

