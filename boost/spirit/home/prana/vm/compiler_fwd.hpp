/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_VM_COMPILER_FWD_HPP)
#define BOOST_SPIRIT_PRANA_VM_COMPILER_FWD_HPP

#include <string>
#include <vector>

#include <boost/spirit/home/prana/vm/core.hpp>
#include <boost/spirit/home/prana/vm/environment/forward.hpp>
#include <boost/spirit/home/prana/vm/environment/function.hpp>

namespace boost {
namespace spirit {
namespace prana {

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

  result_type operator() (spirit::nil) const;
  result_type operator() (utf8_symbol_range const&) const;
  result_type operator() (utree::const_range const&) const;
  result_type operator() (function_base const&) const;
  template<class Value>
    result_type operator() (Value const&) const;

  result_type define_function (std::string const& name,
                               std::vector<std::string>& args,
                               bool fixed_arity, utree const& body);

  function make_lambda (std::vector<std::string> const& args, bool fixed_arity,
                        utree const& body);
};

inline bool is_function_definition (utree const& name);

inline std::string get_symbol (utree const& name);

inline void compile (utree const& ast, compiler_environment& env,
                     function& r, actor_list& fragments,
                     std::size_t parent_line = 1,
                     std::string const& source = "<string>");

inline void compile_program (utree const& ast, compiler_environment& env,
                             actor_list& r, actor_list& fragments,
                             std::size_t parent_line = 1,
                             std::string const& source = "<string>");

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_VM_COMPILER_FWD_HPP

