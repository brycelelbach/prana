/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PHXPR_DB07A041_4FEF_48BA_8182_C8C02F6AA823)
#define PHXPR_DB07A041_4FEF_48BA_8182_C8C02F6AA823

#include <phxpr/config.hpp>

#include <boost/ref.hpp>

#include <phxpr/primitives/actor.hpp>

#include <prana/utree/predicates.hpp> 

namespace phxpr {

struct function_body: actor<function_body> {
  utree code;

  function_body (void): code() { }

  function_body (utree const& code_):
    code(code_) { }

  function_body (function_body const& other):
    code(other.code) { }

  template <typename F>
  function_body (F const& code_):
    code(stored_function<F>(code_)) { }

  utree eval (scope const& env) const {
    if (prana::recursive_which(code) == utree_type::function_type)
      return code.eval(env);
    else
      return utree(boost::ref(code));
  }
};

} // phxpr

#endif // PHXPR_DB07A041_4FEF_48BA_8182_C8C02F6AA823

