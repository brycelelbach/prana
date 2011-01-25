/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_VM_INTERPRETER_FWD_HPP)
#define BSP_VM_INTERPRETER_FWD_HPP

#include <boost/spirit/home/prana/vm/compiler_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {
    
void build_basic_environment (compiler_environment& env);

struct interpreter {
  typedef function result_type;

  template<class Source>
  interpreter (Source& in, std::string const& source_file = "<string>",
               compiler_environment* envp = 0);

  interpreter (utree const& program, compiler_environment* envp = 0);

  function operator() (std::string const& name);

  compiler_environment env;
  utree program;
  actor_list fragments;
  actor_list flist;
};

} // prana
} // spirit
} // boost

#endif // BSP_VM_INTERPRETER_FWD_HPP



