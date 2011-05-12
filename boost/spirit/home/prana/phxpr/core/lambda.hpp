/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_0D84F8E7_6DA2_43E1_8359_A8AF3A32C6CB)
#define BSP_0D84F8E7_6DA2_43E1_8359_A8AF3A32C6CB

#include <boost/spirit/home/prana/config.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>

#include <boost/spirit/home/prana/phxpr/core/composite.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

// TODO: use function for arity/level stuff.
    
struct lambda_function: actor<lambda_function> {
  boost::shared_ptr<function> body;
  boost::shared_ptr<actor_list> elements;
  scope::size_type level;

  typedef utree result_type;

  lambda_function (boost::shared_ptr<actor_list> const& elements_,
                   boost::shared_ptr<function> const& body_,
                   scope::size_type level_ = 0):
    body(body_), elements(elements_), level(level_)
  {
    BOOST_ASSERT(body);
    BOOST_ASSERT(elements);
  }
  
  utree eval (scope const& env) const {
    scope const* outer = &env; // Get the parent scope.

    while (level != outer->level())
      outer = outer->outer();
      
    if (elements && !elements->empty()) {
      boost::scoped_array<utree> fargs(new utree[elements->size()]);

      std::size_t i = 0;

      BOOST_FOREACH(function const& element, *elements) {
        fargs[i++] = element(env);
      }

      utree* fi = fargs.get();

      return body->eval(scope(fi, fi + elements->size(), outer));
    }
    
    else {
      return body->eval(scope(0, 0, outer));
    }
  }
};

struct formals {
  typedef function result_type;

  boost::shared_ptr<actor_list> elements;
  scope::size_type level;

  formals (boost::shared_ptr<actor_list> const& elements_,
           scope::size_type level_):
    elements(elements_), level(level_) { }

  function operator() (function const& body) const {
    return function(lambda_function(elements,
      boost::shared_ptr<function>(new function(body)), level));
  }
};

struct lambda_composite: composite<lambda_composite, formals> {
  scope::size_type const level;

  lambda_composite (scope::size_type level_): level(level_) { }

  formals compose (boost::shared_ptr<actor_list> const& elements) const {
    return formals(elements, level);
  }
};

lambda_composite const lambda = lambda_composite(0);

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_0D84F8E7_6DA2_43E1_8359_A8AF3A32C6CB

