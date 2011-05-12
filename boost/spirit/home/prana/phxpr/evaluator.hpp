////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_B9D84807_053E_486C_A47D_AE22F28D5A9B)
#define BSP_B9D84807_053E_486C_A47D_AE22F28D5A9B

#include <boost/function.hpp>

#include <boost/spirit/home/prana/phxpr/core/actor.hpp>
#include <boost/spirit/home/prana/phxpr/core/function.hpp>
#include <boost/spirit/home/prana/phxpr/macro.hpp>
#include <boost/spirit/home/prana/phxpr/environment.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

// TODO: Wrap all compiled functions in procedures, so that passing around
// intrinsics will work.
typedef boost::function<phxpr::function(actor_list const&)> compiled_function;

struct evaluator {
  environment<compiled_function> variables;
  environment<macro> macros;

  typedef phxpr::function result_type;

  evaluator (void) { }

  evaluator (environment<compiled_function>* variables_parent,
             environment<macro>* macros_parent):
    variables(variables_parent), macros(macros_parent) { }

  ~evaluator (void);

  // forwarder
  result_type operator() (utree const& ut);
}; 
  
} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_B9D84807_053E_486C_A47D_AE22F28D5A9B

