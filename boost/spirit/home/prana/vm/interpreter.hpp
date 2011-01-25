/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_VM_ENVIRONMENT_FWD_HPP)
#define BSP_VM_ENVIRONMENT_FWD_HPP

#include <boost/spirit/home/prana/vm/interpreter_fwd.hpp>
#include <boost/spirit/home/prana/vm/compiler.hpp>
#include <boost/spirit/home/prana/parse/parse_sexpr.hpp>

namespace boost {
namespace spirit {
namespace prana {

void build_basic_environment (compiler_environment& env) {
  env.functions.define("if", if_, 3, true);
  env.functions.define("begin", begin, 1, false);
  env.functions.define("list", list, 1, false);
  env.functions.define("display", display, 1, true);
  env.functions.define("front", front, 1, true);
  env.functions.define("back", back, 1, true);
  env.functions.define("rest", rest, 1, true);
  env.functions.define("!", negate, 1, true);
  env.functions.define("=", equal, 2, true);
  env.functions.define("<", less, 2, true);
  env.functions.define("<=", less_equal, 2, true);
  env.functions.define("+", plus, 2, false);
  env.functions.define("-", minus, 2, false);
  env.functions.define("*", times, 2, false);
  env.functions.define("/", divide, 2, false);
}

template<class Source>
interpreter::interpreter (Source& in, std::string const& source,
                          compiler_environment* envp)
{
  if (envp == 0)
    build_basic_environment(env);

  else
    env = *envp;

  if (parse_sexpr_list(in, program, source)) {
    compile_program(program, env, flist, fragments, source);
  }
}

interpreter::interpreter (utree const& program, compiler_environment* envp) {
  if (envp == 0)
    build_basic_environment(env);
  else
    env = *envp;

  compile_program(program, env, flist, fragments);
}

function interpreter::operator() (std::string const& name) {
  using fusion::at_c;

  function_definition r = env.functions.find(name);

  if (at_c<0>(r)) {
    compiled_function* cf = at_c<0>(r);
    int arity = at_c<1>(r);
    bool fixed_arity = at_c<2>(r);
    actor_list flist;

    if (arity > 0) {
      for (int i = 0; i < (arity - 1); ++i)
        flist.push_back(arg(i));

      if (fixed_arity)
        flist.push_back(arg(arity - 1));
      else
        flist.push_back(varg(arity - 1));
    }

    return (*cf)(flist);
  }

  else
    return function();
}

} // prana
} // spirit
} // boost

#endif // BSP_VM_ENVIRONMENT_FWD_HPP

