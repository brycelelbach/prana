////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_90FE26BE_2CE4_4B6F_AB42_505F8B5D0537)
#define PHXPR_90FE26BE_2CE4_4B6F_AB42_505F8B5D0537

#include <phxpr/config.hpp>

#include <sheol/adt/dynamic_array.hpp>

#include <phxpr/primitives/actor.hpp>
#include <phxpr/signature.hpp>

namespace phxpr {

struct variable_reference: actor<variable_reference> {
  typedef sheol::adt::dynamic_array<signature> gpt_type;

  phxpr::shared_ptr<utree> value;
  phxpr::shared_ptr<gpt_type> global_procedure_table;

  variable_reference (utree const& value_,
                      phxpr::shared_ptr<gpt_type> const& gpt):
    value(phxpr::make_shared<utree>(value_)), global_procedure_table(gpt)
  { }
 
  variable_reference (phxpr::shared_ptr<utree> const& value_,
                      phxpr::shared_ptr<gpt_type> const& gpt):
    value(value_), global_procedure_table(gpt)
  { }

/*  
  ~variable_reference (void) {
    if (value.unique())
      value->clear();
  }
*/

  // TODO: Refactor this code with the code in procedure
  utree eval (utree& ut) const { 
    BOOST_ASSERT(value);

#if 1
    using boost::fusion::at_c;
   
    runtime_environment& env = *ut.get<runtime_environment*>();

    if (prana::recursive_which(*value) == utree_type::function_type) {
      BOOST_ASSERT(value->tag() <= global_procedure_table->size());

      // Load the argument's signature from the gpt.
      signature const& sig = (*global_procedure_table)[value->tag()];

//      std::cout << at_c<3>(sig) << std::endl;

      if ((at_c<3>(sig) == function_type::placeholder)||
          (at_c<3>(sig) == function_type::thunk))
        return utree(new variable_reference
          (env.invoke(value), global_procedure_table));
    }
#endif

    return *value;
  }

  // REVIEW: Will passing shallow references via smart pointers screw with
  // assignment once it's implemented?
  function_base* copy (void) const
  { return new variable_reference(value, global_procedure_table); }
};

struct recursive_reference: actor<recursive_reference> {
  typedef sheol::adt::dynamic_array<signature> gpt_type;

  phxpr::shared_ptr<utree> const& value;
  phxpr::shared_ptr<gpt_type> global_procedure_table;
 
  recursive_reference (phxpr::shared_ptr<utree> const& value_,
                       phxpr::shared_ptr<gpt_type> const& gpt):
    value(value_), global_procedure_table(gpt)
  { }

  // TODO: Refactor this code with the code in procedure
  utree eval (utree& ut) const { 
    BOOST_ASSERT(value);

    using boost::fusion::at_c;
   
    runtime_environment& env = *ut.get<runtime_environment*>();

    if (prana::recursive_which(*value) == utree_type::function_type) {
      BOOST_ASSERT(value->tag() <= global_procedure_table->size());

      // Load the argument's signature from the gpt.
      signature const& sig = (*global_procedure_table)[value->tag()];

//      std::cout << at_c<3>(sig) << std::endl;

#if 0
      if ((at_c<3>(sig) == function_type::placeholder)||
          (at_c<3>(sig) == function_type::thunk))
        return utree(new recursive_reference
          (env.invoke(value), global_procedure_table));
#endif
    }

    return *value;
  }

  // REVIEW: Will passing shallow references via smart pointers screw with
  // assignment once it's implemented?
  function_base* copy (void) const
  { return new recursive_reference(value, global_procedure_table); }
};

} // phxpr

#endif // PHXPR_90FE26BE_2CE4_4B6F_AB42_505F8B5D0537

