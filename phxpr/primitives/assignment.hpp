////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_F0785E73_5C94_497C_B57E_21A083F52D86)
#define PHXPR_F0785E73_5C94_497C_B57E_21A083F52D86

#include <phxpr/config.hpp>

#include <phxpr/signature.hpp>
#include <phxpr/primitives/actor.hpp>
#include <phxpr/exception.hpp>

namespace phxpr {

struct local_assignment: actor<local_assignment> {
  boost::shared_ptr<utree> value;
  displacement n;
  displacement frame;

  local_assignment (utree const& value_, displacement n_, displacement frame_):
    value(boost::make_shared<utree>(value_)), n(n_), frame(frame_)
  { }
 
  local_assignment (boost::shared_ptr<utree> const& value_, displacement n_,
                    displacement frame_):
    value(value_), n(n_), frame(frame_)
  { }

  utree eval (utree& ut) const {
    boost::shared_ptr<runtime_environment> eptr
      = ut.get<runtime_environment*>()->checkout();

    if (!eptr) {
      BOOST_THROW_EXCEPTION
        (invalid_local_assignment(n, frame, eptr, arity_type::fixed));
    }

    while (frame != eptr->level()) {
      if (!eptr->outer()) {
        BOOST_THROW_EXCEPTION
          (invalid_local_assignment(n, frame, eptr, arity_type::fixed));
      }

      eptr = eptr->outer();
    } 

    if (eptr->size() <= n) {
      BOOST_THROW_EXCEPTION
        (invalid_local_assignment(n, frame, eptr, arity_type::fixed));
    }

    (*eptr)[n] = eptr->invoke(value);

    // assignment returns unspecified
    return utree(); 
  }

  // REVIEW: Will passing shallow references via smart pointers screw things up?
  function_base* copy (void) const
  { return new local_assignment(value, n, frame); }
};

} // phxpr

#endif // PHXPR_F0785E73_5C94_497C_B57E_21A083F52D86

