#include <vector>
#include <map>
#include <exception>

#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/lexical_cast.hpp>
#include <scheme/intrinsics.hpp>
#include <scheme/interpreter.hpp>
#include <input/parse_sexpr.hpp>

///////////////////////////////////////////////////////////////////////////////
//  The compiler
///////////////////////////////////////////////////////////////////////////////
function compile(
  utree const& ast,
  environment& env,
  actor_list& fragments,
  int parent_line,
  std::string const& source_file = "");

struct external_function : composite<external_function> {
  // we must hold f by reference because functions can be recursive
  boost::reference_wrapper<function const> f;
  int level;

  external_function(function const& f, int level)
    : f(f), level(level) {}

  using base_type::operator();
  function operator()(actor_list const& elements) const {
    return function(lambda_function(f, elements, level));
  }
};

struct compiler {
  typedef function result_type;
  environment& env;
  actor_list& fragments;
  int line;
  std::string source_file;

  compiler(
    environment& env,
    actor_list& fragments,
    int line,
    std::string const& source_file = "")
    : env(env), fragments(fragments),
      line(line), source_file(source_file) {
  }

  function operator()(nil) const {
    return scheme::val(utree());
  }

  template <typename T>
  function operator()(T const& val) const {
    return scheme::val(utree(val));
  }

  function operator()(utf8_symbol_range const& str) const {
    std::string name(str.begin(), str.end());
    boost::tuple<compiled_function*, int, bool> r = env.find(name);

    if (boost::get<0>(r)) {
      actor_list flist;
      return (*boost::get<0>(r))(flist);
    }

    throw identifier_not_found(name);
    return function();
  }

  function make_lambda(
    std::vector<std::string> const& args,
    bool fixed_arity,
    utree const& body) const {
    environment local_env(&this->env);

    for (std::size_t i = 0; i < args.size(); ++i) {
      if (!fixed_arity && (args.size() - 1) == i)
        local_env.define(args[i],
            boost::bind(varg, i, local_env.level()), 0, false);
      else
        local_env.define(args[i],
            boost::bind(arg, i, local_env.level()), 0, false);
    }

    actor_list flist;

    if (body.size() == 0)
      return function();

    //~ throw no_body();
    BOOST_FOREACH(utree const & item, body) {
      function f = compile(item, local_env, fragments, line, source_file);

      if (!is_define(item))
        flist.push_back(f);
    }

    if (flist.size() > 1)
      return protect(block(flist));
    else
      return protect(flist.front());
  }

  static bool is_define(utree const& item) {
    if (item.which() != utree_type::list_type ||
        item.begin()->which() != utree_type::symbol_type)
      return false;

    return get_symbol(*item.begin()) == "define";
  }

  function define_function(
    std::string const& name,
    std::vector<std::string>& args,
    bool fixed_arity,
    utree const& body) const {
    try {
      function* fp = 0;

      if (env.defined(name)) {
        fp = env.find_forward(name);

        if (fp != 0 && !fp->empty())
          throw body_already_defined(name);
      }

      if (fp == 0) {
        fragments.push_back(function());
        fp = &fragments.back();
        env.define(name, external_function(*fp, env.level()), args.size(), fixed_arity);
      }

      function lambda = make_lambda(args, fixed_arity, body);

      if (!lambda.empty()) {
        // unprotect (eval returns a function)
        *fp = lambda();
      } else {
        // allow forward declaration of scheme functions
        env.forward_declare(name, fp);
      }

      return *fp;
    } catch (std::exception const&) {
      env.undefine(name);
      throw;
    }
  }

  function operator()(utree::const_range const& range) const {
    typedef utree::const_range::iterator iterator;

    if (range.begin()->which() != utree_type::symbol_type)
      throw function_application_expected(*range.begin());

    std::string name(get_symbol(*range.begin()));

    if (name == "quote") {
      iterator i = range.begin();
      ++i;
      return scheme::val(*i);
    }

    if (name == "define") {
      std::string fname;
      std::vector<std::string> args;
      bool fixed_arity = true;
      iterator i = range.begin();
      ++i;

      if (i->which() == utree_type::list_type) {
        // (define (f x) ...body...)
        utree const& decl = *i++;
        iterator di = decl.begin();
        fname = get_symbol(*di++);

        while (di != decl.end()) {
          std::string sym = get_symbol(*di++);

          if (sym == ".")
            // check that . is one pos behind the last arg
            fixed_arity = false;
          else
            args.push_back(sym);
        }
      } else {
        // (define f ...body...)
        fname = get_symbol(*i++);

        // (define f (lambda (x) ...body...))
        if (i != range.end()
            && i->which() == utree_type::list_type
            && get_symbol((*i)[0]) == "lambda") {
          utree const& arg_names = (*i)[1];
          iterator ai = arg_names.begin();

          while (ai != arg_names.end()) {
            std::string sym = get_symbol(*ai++);

            if (sym == ".")
              // check that . is one pos behind the last arg
              fixed_arity = false;
            else
              args.push_back(sym);
          };

          iterator bi = i->begin();

          ++bi;

          ++bi; // (*i)[2]

          utree body(utree::const_range(bi, i->end()), shallow);

          return define_function(fname, args, fixed_arity, body);
        }
      }

      utree body(utree::const_range(i, range.end()), shallow);
      return define_function(fname, args, fixed_arity, body);
    }

    if (name == "lambda") {
      // (lambda (x) ...body...)
      iterator i = range.begin();
      ++i;
      utree const& arg_names = *i++;
      iterator ai = arg_names.begin();
      std::vector<std::string> args;
      bool fixed_arity = true;

      while (ai != arg_names.end()) {
        std::string sym = get_symbol(*ai++);

        if (sym == ".")
          // check that . is one pos behind the last arg
          fixed_arity = false;
        else
          args.push_back(sym);
      }

      utree body(utree::const_range(i, range.end()), shallow);
      return make_lambda(args, fixed_arity, body);
    }

    // (f x)
    boost::tuple<compiled_function*, int, bool> r = env.find(name);

    if (boost::get<0>(r)) {
      compiled_function* cf = boost::get<0>(r);
      int arity = boost::get<1>(r);
      bool fixed_arity = boost::get<2>(r);
      actor_list flist;
      iterator i = range.begin();
      ++i;
      int size = 0;

      for (; i != range.end(); ++i, ++size) {
        flist.push_back(
          compile(*i, env, fragments, line, source_file));
      }

      // Arity check
      if (!fixed_arity) { // non-fixed arity
        if (size < arity)
          throw incorrect_arity(name, arity, false);
      } else { // fixed arity
        if (size != arity)
          throw incorrect_arity(name, arity, true);
      }

      return (*cf)(flist);
    } else {
      throw identifier_not_found(name);
    }

    // Can't reach here
    throw compilation_error();
    return function();
  }

  function operator()(function_base const& pf) const {
    // Can't reach here. Surely, at this point, we don't have
    // utree functions yet. The utree AST should be pure data.
    throw compilation_error();
    return function();
  }

  static std::string get_symbol(utree const& s) {
    if (s.which() != utree_type::symbol_type)
      throw identifier_expected(s);

    utf8_symbol_range symbol = s.get<utf8_symbol_range>();
    return std::string(symbol.begin(), symbol.end());
  }
};

inline function compile(
  utree const& ast,
  environment& env,
  actor_list& fragments,
  int parent_line,
  std::string const& source_file) {
  int line = (ast.which() == utree_type::list_type)
      ? ast.tag() : parent_line;

  try {
    return utree::visit(ast,
        compiler(env, fragments, line, source_file));
  } catch (scheme_exception const& x) {
    if (source_file != "")
      std::cerr << source_file;

    if (line != -1)
      std::cerr << '(' << line << ')';

    std::cerr << " : Error! "  << x.what() << std::endl;
    throw compilation_error();
  }

  return function();
}

void compile_all(
  utree const& ast,
  environment& env,
  actor_list& results,
  actor_list& fragments,
  std::string const& source_file = "") {
  int line = (ast.which() == utree_type::list_type)
      ? ast.tag() : 1;
  BOOST_FOREACH(utree const & program, ast) {
    scheme::function f;

    try {
      if (!compiler::is_define(program)) {
        if (source_file != "")
          std::cerr << source_file;

        int progline = (program.which() == utree_type::list_type)
            ? program.tag() : line;
        std::cerr << '(' << progline << ')';
        std::cerr << " : Error! scheme: Function definition expected." << std::endl;
        continue; // try the next expression
      } else {
        f = compile(program, env, fragments, line, source_file);
      }
    } catch (compilation_error const&) {
      continue; // try the next expression
    }

    results.push_back(f);
  }
}


