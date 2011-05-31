////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_FFA6AC4D_1CA2_493B_8E2D_B391825F49F0)
#define PHXPR_FFA6AC4D_1CA2_493B_8E2D_B391825F49F0

#include <phxpr/config.hpp>

#include <prana/utree/predicates.hpp>

#include <phxpr/predicate.hpp>
#include <phxpr/primitives/actor.hpp>

namespace phxpr {

struct if_: actor<if_> {
  boost::shared_ptr<utree> test;
  boost::shared_ptr<utree> then;

  if_ (utree const& test_, utree const& then_):
    test(boost::make_shared<utree>(test_)),
    then(boost::make_shared<utree>(then_))
  { }
 
  if_ (boost::shared_ptr<utree> const& test_,
       boost::shared_ptr<utree> const& then_):
    test(test_), then(then_)
  { }

  utree eval (utree& ut) const {
    if (predicate(test->eval(ut)))
      return then->eval(ut);
    return utree();
  }

  // REVIEW: Will passing shallow references via smart pointers screw with
  // assignment once it's implemented?
  function_base* copy (void) const
  { return new if_(test, then); }
};

struct if_else: actor<if_else> {
  boost::shared_ptr<utree> test;
  boost::shared_ptr<utree> then;
  boost::shared_ptr<utree> else_;

  if_else (utree const& test_, utree const& then_, utree const& e):
    test(boost::make_shared<utree>(test_)),
    then(boost::make_shared<utree>(then_)),
    else_(boost::make_shared<utree>(e))
  { }
 
  if_else (boost::shared_ptr<utree> const& test_,
           boost::shared_ptr<utree> const& then_,
           boost::shared_ptr<utree> const& e):
    test(test_), then(then_), else_(e)
  { }

  utree eval (utree& ut) const {
    if (predicate(test->eval(ut)))
      return then->eval(ut); 
   return else_->eval(ut);
  }

  // REVIEW: Will passing shallow references via smart pointers screw with
  // assignment once it's implemented?
  function_base* copy (void) const
  { return new if_else(test, then, else_); }
};

} // phxpr

#endif // PHXPR_FFA6AC4D_1CA2_493B_8E2D_B391825F49F0

