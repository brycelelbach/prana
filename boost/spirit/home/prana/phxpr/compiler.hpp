/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_PHXPR_COMPILER_HPP)
#define BSP_PHXPR_COMPILER_HPP

#include <boost/bind.hpp>

#include <boost/spirit/home/prana/phxpr/compiler_fwd.hpp>
#include <boost/spirit/home/prana/phxpr/get_symbol.hpp>
#include <boost/spirit/home/prana/phxpr/intrinsics.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

compiler::compiler (compiler_environment& env, actor_list& fragments,
                    std::size_t line, std::string const& source):
                      env(env),
                      fragments(fragments),
                      line(line),
                      source(source) { }

function compiler::operator() (utree::nil_type) const {
  return quote(utree());
}

function compiler::operator() (utf8_symbol_range_type const& str) const {
  using fusion::at_c;

  std::string name(str.begin(), str.end());

  // Find the variable in the environment.
  function_definition r = env.functions(name); 

  if (at_c<0>(r)) {
    actor_list flist;
    return (*at_c<0>(r))(flist);
  }

  throw identifier_not_found(name);
  return function();
}

function compiler::operator() (utree::const_range const& range) const {
  using fusion::at_c;

  typedef utree::const_range::iterator iterator;

  // (<keyword> <datum> ...) 
  // TODO: macros

  std::string name(get_symbol(*range.begin()));

  if (range.begin()->which() != utree_type::symbol_type)
    throw expected_function_application(*range.begin());
  
  if ((name == "phx:'") || (name == "phx:quote")) {
    iterator i = range.begin(); ++i;
    return quote(*i);
  }

  if (name == "phx:define") {
    std::string fname;
    std::vector<std::string> args;
    bool fixed_arity = true;
    iterator i = range.begin(); ++i;

    if (i->which() == utree_type::list_type) {
      // (define (f x) ...body...)
      utree const& decl = *i++;
      iterator di = decl.begin();
      fname = get_symbol(*di++);

      while (di != decl.end()) {
        std::string sym = get_symbol(*di++);

        if (sym == "phx:...")
          // check that . is one pos behind the last arg
          fixed_arity = false;
        else
          args.push_back(sym);
      }
    }

    else {
      // (define f ...body...)
      fname = get_symbol(*i++);

      // (define f (lambda (x) ...body...))
      if (i != range.end()
          && i->which() == utree_type::list_type
          && get_symbol((*i)[0]) == "phx:lambda") {
        utree const& arg_names = (*i)[1];
        iterator ai = arg_names.begin();

        while (ai != arg_names.end()) {
          std::string sym = get_symbol(*ai++);

          if (sym == "phx:...")
            // check that . is one pos behind the last arg
            fixed_arity = false;
          else
            args.push_back(sym);
        };

        iterator bi = i->begin(); ++i; ++i; // (*i)[2]

        utree body(utree::const_range(bi, i->end()), shallow);

        return define_function(fname, args, fixed_arity, body);
      }
    }

    utree body(utree::const_range(i, range.end()), shallow);
    return define_function(fname, args, fixed_arity, body);
  }

  if (name == "phx:lambda") {
    // (lambda (x) ...body...)
    iterator i = range.begin(); ++i;
    utree const& arg_names = *i++;
    iterator ai = arg_names.begin();
    std::vector<std::string> args;
    bool fixed_arity = true;

    while (ai != arg_names.end()) {
      std::string sym = get_symbol(*ai++);

      if (sym == "phx:...")
        // check that . is one pos behind the last arg
        fixed_arity = false;
      else
        args.push_back(sym);
    }

    utree body(utree::const_range(i, range.end()), shallow);
    return make_lambda(args, fixed_arity, body);
  }

  function_definition r = env.functions(name);

  if (at_c<0>(r)) {
    compiled_function* cf = at_c<0>(r);
    unsigned arity = at_c<1>(r);
    bool fixed_arity = at_c<2>(r);

    actor_list flist;

    iterator i = range.begin(); ++i;
    unsigned size = 0;

    for (; i != range.end(); ++i, ++size) {
      flist.push_back(function());
      compile(*i, env, flist.back(), fragments, source, line);
    }

    if ((!fixed_arity) && (size < arity)) // Non-fixed arity.
      throw incorrect_arity(name, arity, false);
    else if (size != arity) // Fixed arity.
      throw incorrect_arity(name, arity, true);

    return (*cf)(flist);
  }
  
  throw identifier_not_found(name);
}

function compiler::operator() (function_base const& pf) const {
  // Can't reach here. Surely, at this point, we don't have
  // utree functions yet. The utree AST should be pure data.
  throw compilation_error();
  return function();
}

template<class Value>
function compiler::operator() (Value const& val_) const {
  return quote(utree(val_));
}

function compiler::define_function (std::string const& name,
                                    std::vector<std::string>& args,
                                    bool fixed_arity, utree const& body) const
{
  try {
    function* fp = 0;

    if (env.functions.defined(name)) {
      fp = env.forwards(name);

      if (fp && !fp->empty())
        throw body_already_defined(name);
    }

    if (!fp) {
      fragments.push_back(function());
      fp = &fragments.back();
      env.functions.define(
        name,
        external_function(*fp, env.functions.level()),
        args.size(),
        fixed_arity
      );
    }

    function lambda = make_lambda(args, fixed_arity, body);

    if (!lambda.empty())
      *fp = lambda(); // unprotect (eval returns a function)
    else // Allow forward declaration of scheme functions.
      env.forwards.define(name, fp);

    return *fp;
  }

  catch (std::exception const&) {
    env.functions.undefine(name);
    throw;
  }
}

function compiler::make_lambda (std::vector<std::string> const& args,
                                bool fixed_arity, utree const& body) const
{
  compiler_environment local_env(this->env);

  for (std::size_t i = 0; i < args.size(); ++i) {
    if (!fixed_arity && (args.size() - 1) == i)
      local_env.functions.define(
        args[i], boost::bind(phxpr::varg, i, local_env.functions.level()),
        0, false
      );
    else
      local_env.functions.define(
        args[i], boost::bind(phxpr::arg, i, local_env.functions.level()),
        0, false
      );
  }

  actor_list flist;

  if (body.size() == 0)
    return function();

  BOOST_FOREACH(utree const& item, body) {
    function f;
    compile(item, local_env, f, fragments, source, line);

    if (!is_function_definition(item))
      flist.push_back(f);
  }

  if (flist.size() > 1)
    return quote(phxpr::begin(flist));
  else
    return quote(flist.front());
}

inline bool is_function_definition (utree const& item) {
  if (  item.which() != utree_type::list_type
     || item.begin()->which() != utree_type::symbol_type)
    return false;

  return get_symbol(*item.begin()) == "phx:define";
}

inline bool is_syntax_definition (utree const& item) {
  if (  item.which() != utree_type::list_type
     || item.begin()->which() != utree_type::symbol_type)
    return false;

  return get_symbol(*item.begin()) == "phx:define-syntax";
}

inline void compile (utree const& ast, compiler_environment& env,
                     function& r, actor_list& fragments,
                     std::string const& source, std::size_t parent_line)
{
  std::size_t line = (ast.which() == utree_type::list_type)
                   ? ast.tag()
                   : parent_line;

  try { r = utree::visit(ast, compiler(env, fragments, line, source)); }

  catch (utree_exception const& x) {
    std::cerr << "(exception \""
              << source << "\" " << ((line == -1) ? -1 : line)
              << " '(compilation-error \"" << x.what() << "\"))\n";
  }
}

inline void compile_program (utree const& ast, compiler_environment& env,
                             actor_list& r, actor_list& fragments,
                             std::string const& source, std::size_t parent_line)
{
  std::size_t line = (ast.which() == utree_type::list_type)
                   ? ast.tag()
                   : parent_line;

  BOOST_FOREACH(utree const& local_ast, ast) {
    function f;

    try {
      if (!is_function_definition(local_ast)) {
        std::size_t local_line = (local_ast.which() == utree_type::list_type)
                               ? local_ast.tag()
                               : line;
    
        std::cerr << "(exception \""
                  << source << "\" " << ((line == -1) ? -1 : line)
                  << " '(function-definition-expected " << local_ast << "))\n";
        continue; // Try the next expression.
      }

      else 
        compile(local_ast, env, f, fragments, source, line);
    }
    
    catch (compilation_error const&) {
      continue; // Try the next expression.
    }

    r.push_back(f);
  }
}

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_PHXPR_COMPILER_HPP

