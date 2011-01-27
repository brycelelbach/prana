/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_COMPILER_FWD_HPP)
#define BSP_PHXPR_COMPILER_FWD_HPP

#include <string>
#include <vector>

#include <boost/spirit/home/prana/phxpr/core.hpp>
#include <boost/spirit/home/prana/phxpr/environment.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct compiler_environment {
  compiler_environment (void) { }

  compiler_environment (compiler_environment& parent):
    functions(&parent.functions),
    forwards(&parent.forwards) { }

  function_environment functions;
  forward_environment forwards;
};

struct compiler {
  compiler_environment& env;
  actor_list& fragments;
  std::size_t line;
  std::string source;

  typedef function result_type;

  compiler (compiler_environment& env, actor_list& fragments,
            std::size_t line = 1, std::string const& source = "<string>");

  result_type operator() (utree::nil_type) const;
  result_type operator() (utf8_symbol_range_type const&) const;
  result_type operator() (utree::const_range const&) const;
  result_type operator() (function_base const&) const;
  template<class Value>
    result_type operator() (Value const&) const;

  result_type define_function (std::string const& name,
                               std::vector<std::string>& args,
                               bool fixed_arity, utree const& body) const;

  function make_lambda (std::vector<std::string> const& args, bool fixed_arity,
                        utree const& body) const;
};

inline bool is_function_definition (utree const& name);

inline void compile (utree const& ast, compiler_environment& env,
                     function& r, actor_list& fragments,
                     std::string const& source = "<string>",
                     std::size_t parent_line = 1);

inline void compile_program (utree const& ast, compiler_environment& env,
                             actor_list& r, actor_list& fragments,
                             std::string const& source = "<string>",
                             std::size_t parent_line = 1);

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_COMPILER_FWD_HPP

