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

#include <boost/make_shared.hpp>
#include <boost/ref.hpp>

#include <sheol/adt/dynamic_array.hpp>

#include <phxpr/primitives/actor.hpp>

#include <prana/utree/predicates.hpp> 

namespace phxpr {

struct function_body: actor<function_body> {
  typedef sheol::adt::dynamic_array<utree> code_type;

  boost::shared_ptr<code_type> code;

  function_body (void): code() { }

  function_body (utree const& code_): code(boost::make_shared<code_type>())
  { code->push_back(code_); }
  
  function_body (boost::shared_ptr<code_type> const& code_): code(code_) { }

  function_body (function_body const& other): code(other.code) { }

  template <typename F>
  function_body (F const& code_): code(boost::make_shared<code_type>())
  { code->push_back(utree(stored_function<F>(code_))); }

  utree eval (scope const& env) const {
    BOOST_ASSERT(code);
    BOOST_ASSERT(code->size());

    code_type::size_type i = 0;
    const code_type::size_type end = code->size();

    for (; i != (end - 1); ++i) {
      if (prana::recursive_which((*code)[i]) == utree_type::function_type)
        (*code)[i].eval(env);
    }

    if (prana::recursive_which((*code)[end - 1]) == utree_type::function_type)
      return (*code)[end - 1].eval(env);
    else
      return utree(boost::ref((*code)[end - 1]));
  }
};

} // phxpr

#endif // PHXPR_DB07A041_4FEF_48BA_8182_C8C02F6AA823

