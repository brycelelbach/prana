////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/assert.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <boost/spirit/home/prana/support/utree_function_cast.hpp>
#include <boost/spirit/home/prana/support/utree_predicates.hpp>
#include <boost/spirit/home/prana/phxpr/core/arguments.hpp>
#include <boost/spirit/home/prana/phxpr/core/variable_arguments.hpp>
#include <boost/spirit/home/prana/phxpr/core/quote.hpp>
#include <boost/spirit/home/prana/phxpr/core/procedure.hpp>
#include <boost/spirit/home/prana/phxpr/intrinsics/begin.hpp>
#include <boost/spirit/home/prana/phxpr/evaluator.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

// TODO: don't use temporary utree ranges!
// TODO: return shared_ptr<>s to functions instead of functions
// TODO: make use of visit_ref

std::string get_symbol(utree const& s) {
  if (s.which() != utree_type::symbol_type) {
    // TODO: make this an exception
    BOOST_ASSERT(false);
  }
 
  utf8_symbol_range_type symbol = s.get<utf8_symbol_range_type>();
  return std::string(symbol.begin(), symbol.end());
}

bool is_define (utree const& item) {
  if (item.which() != utree_type::list_type ||
      item.begin()->which() != utree_type::symbol_type)
    return false;
  return (get_symbol(*item.begin()) == "variable")
      || (get_symbol(*item.begin()) == "macro");
}

struct evaluator_visitor {
  // FIXME: use of visit_ref will mitigate the need for these members to be 
  // mutable. 
  mutable environment<compiled_function>* variables;
  mutable environment<macro>* macros;

  typedef phxpr::function result_type;

  evaluator_visitor (void): variables(0), macros(0) { }

  evaluator_visitor (environment<compiled_function>* variables_,
                     environment<macro>* macros_):
    variables(variables_),
    macros(macros_) { }

  evaluator_visitor (evaluator_visitor const& other):
    variables(other.variables),
    macros(other.macros) { } 

  // forwarder
  result_type operator() (utree const& ut) const
  { return utree::visit(ut, *this); }
  
  result_type operator() (utree::invalid_type) const {
    // TODO: make this an exception
    BOOST_ASSERT(false);
    return result_type();
  }

  template<class T>
  result_type operator() (T const& val) const
  { return result_type(utree(val)); }

  result_type operator() (utf8_symbol_range_type const& str) const {
    // {{{ variable reference implementation
    using boost::fusion::at_c;

    std::string sym(str.begin(), str.end());

    // TODO: make this an exception
    BOOST_ASSERT(sym != "variable");
    BOOST_ASSERT(sym != "macro");
    BOOST_ASSERT(sym != "quote");
    BOOST_ASSERT(sym != "lambda");

    // {{{ variable expansion
    fusion::vector3<
      environment<compiled_function>::const_iterator, bool, scope::size_type
    > func = (*variables)[sym];

    if (at_c<1>(func)) {
      if (at_c<0>(func)->second.target<placeholder>()) {
        placeholder const* p = at_c<0>(func)->second.target<placeholder>();
        return function(p->body->f, p->body->arity, p->body->fixed, p->level);
      }

      else if (at_c<0>(func)->second.target<procedure>()) {
        procedure const* p = at_c<0>(func)->second.target<procedure>();
        return quote(*p->body);
      }

      // TODO: eliminate this case, by storing all compiled functions in
      // procedure instances.
      return (at_c<0>(func)->second)(boost::shared_ptr<actor_list>());
    } // }}}
    
    // TODO: replace with exception (identifier not found error)
    BOOST_ASSERT(false);
    return result_type();
  } // }}}

  template<class Iterator>
  result_type operator() (iterator_range<Iterator> const& range) const;

  result_type operator() (any_ptr const&) const {
    // TODO: make this an exception
    BOOST_ASSERT(false);
    // REVIEW: right return type?
    return result_type();
  }

  result_type operator() (function_base const&) const {
    // TODO: make this an exception
    BOOST_ASSERT(false);
    // REVIEW: right return type?
    return result_type();
  }

  result_type define_variable (utree const& ut) const {
    // {{{ variable definitions
    utree::const_iterator it = ut.begin(), end = ut.end();
   
    // TODO: replace with exception
    BOOST_ASSERT(it != end);

    std::string name = get_symbol(*it);
    ++it;
    
    // TODO: replace with exception
    BOOST_ASSERT(it != end);
    
    // TODO: replace with exception (multiple definitions)
    BOOST_ASSERT(!variables->locally_defined(name));

    boost::shared_ptr<phxpr::function> func(new phxpr::function);
    variables->define(name, procedure(func, variables->level()));
    
    function r = (*this)(*it);
    
    func->f = r.f;
    func->arity = r.arity; 
    func->fixed = r.fixed;
    func->level = r.level;
 
    return result_type();  
  } // }}}
  
  result_type define_macro (utree const& ut) const {
    // {{{ IMPLEMENT macro definitions
    utree::const_iterator it = ut.begin(), end = ut.end();
    
    // TODO: replace with exception
    BOOST_ASSERT(it != end);

    std::string name = get_symbol(*it);
    ++it;

    // TODO: replace with exception
    BOOST_ASSERT(it != end);

    // TODO: replace with exception (multiple definitions)
    BOOST_ASSERT(!macros->locally_defined(name));

    return result_type(); 
  } // }}}
  
  result_type make_lambda (utree const& ut) const;

  template<class Iterator>
  result_type make_anonymous_call (iterator_range<Iterator> const& range) const 
  { // {{{ anonymous lambda or indirect procedure call 
    typename iterator_range<Iterator>::const_iterator it = range.begin(),
                                                      end = range.end();

    boost::shared_ptr<function> anon(new function);
    *anon = utree::visit(*it, *this);
    
    ++it;

    utree use(iterator_range<Iterator>(it, end), spirit::shallow);

    boost::shared_ptr<actor_list> flist(new actor_list);

    BOOST_FOREACH(utree const& e, use) {
      flist->push_back(utree::visit(e, *this));
    }

    return lambda_function(flist, anon, variables->level()); 
  } // }}}

  template<class Iterator>
  fusion::vector2<result_type, bool>
  make_named_call (std::string const& sym,
                   iterator_range<Iterator> const& range) const;

  template<class Iterator>
  fusion::vector2<result_type, bool>
  expand_macro (std::string const& sym,
                iterator_range<Iterator> const& range) const
  { // {{{ macro expansion implementation
    using boost::fusion::at_c;

    fusion::vector3<
      environment<macro>::const_iterator, bool, scope::size_type
    > macro = (*macros)[sym];

    if (at_c<1>(macro)) {
      utree use(range, spirit::shallow);

      boost::shared_ptr<matcher> match = at_c<0>(macro)->second.match(use);

      if (match) {
        boost::shared_ptr<utree> expansion = match->expand();
        // TODO: make this an exception
        BOOST_ASSERT(expansion);
        return fusion::vector2<result_type, bool>
          (utree::visit(*expansion, *this), true);
      }
    }

    return fusion::vector2<result_type, bool>(result_type(), false);
  } // }}}
}; 

evaluator::~evaluator (void) {
  // {{{ tear down code for refcnt'd recursive calls
  if (variables.level() == 0) {
    // Clean up recursive calls. Without help, shared_ptr<> won't release
    // these properly.
    BOOST_FOREACH(environment<compiled_function>::value_type const& e,
                  variables.definitions)
    {
      if (e.second.target<procedure>())
        e.second.target<procedure>()->body->f.clear();
      else if (e.second.target<placeholder>())
        e.second.target<placeholder>()->body->f.clear();
    } 
  }
} // }}}

// REVIEW: Move me somewhere more appropriate?
// TODO: Pass in more generic parameters instead of a placeholder instance
struct trampoline_function: actor<trampoline_function> {
  // {{{ procedure call trampoline
  boost::shared_ptr<actor_list> elements;
  boost::shared_ptr<placeholder> ph;

  typedef utree result_type;

  trampoline_function (boost::shared_ptr<actor_list> const& elements_,
                       placeholder const& ph_):
    elements(elements_), ph(new placeholder(ph_.body, ph_.level))
  {
    BOOST_ASSERT(elements);
    BOOST_ASSERT(ph);
  }
  
  utree eval (scope const& env) const {
    // TODO: replace with exception
    BOOST_ASSERT(ph);

    scope const* outer = &env; // Get the parent scope.

    scope::size_type const level = (ph->level) ? (ph->level - 1) : 0;

    while (level != outer->level())
      outer = outer->outer();

    // TODO: replace with exception
    BOOST_ASSERT(recursive_which(ph->body->f) == utree_type::function_type);

    // FIXME: avoid dynamic_cast
    stored_function<argument_function<true> > const* scoped_arg
      = utree_function_cast<stored_function<argument_function<true> > >
        (ph->body->f);

    // REVIEW: handle unscoped arg, scoped varg and unscoped varg?
    BOOST_ASSERT(scoped_arg);

    utree real_f = scoped_arg->f.get(env);

    if (elements && !elements->empty()) {
      boost::scoped_array<utree> fargs(new utree[elements->size()]);

      std::size_t i = 0;

      BOOST_FOREACH(function const& element, *elements) {
        fargs[i++] = element(env);
      }

      utree* fi = fargs.get();

      return real_f.eval(scope(fi, fi + elements->size(), outer));
    }
    
    else 
      return real_f.eval(scope(0, 0, outer));
  }
}; // }}}

struct argument_visitor {
  // {{{ visitor that places trampolines for anonymous function arguments
  // FIXME: use of visit_ref will mitigate the need for this member to be 
  // mutable. 
  mutable evaluator_visitor* eval; 
 
  typedef evaluator_visitor::result_type result_type;

  argument_visitor (void): eval(0) { }

  argument_visitor (evaluator_visitor* eval_):
    eval(eval_) { }

  argument_visitor (argument_visitor const& other):
    eval(other.eval) { }
  
  // forwarder
  result_type operator() (utree const& ut) const
  { return utree::visit(ut, *this); }
  
  result_type operator() (utree::invalid_type ivld) const
  { return (*eval)(ivld); }
  
  template<class T>
  result_type operator() (T const& val) const
  { return (*eval)(val); }
  
  result_type operator() (utf8_symbol_range_type const& str) const
  { return (*eval)(str); }
  
  template<class Iterator>
  result_type operator() (iterator_range<Iterator> const& range) const {
    // {{{ IMPLEMENT special handling for lambda expressions as arguments
    return result_type();
  } // }}}

  result_type operator() (any_ptr const& p) const
  { return (*eval)(p); }

  result_type operator() (function_base const& pf) const
  { return (*eval)(pf); }
  // }}}
};
 
template<class Iterator>
fusion::vector2<evaluator_visitor::result_type, bool>
evaluator_visitor::make_named_call (std::string const& sym,
                                    iterator_range<Iterator> const& range) const
{ // {{{ function creation
  using boost::fusion::at_c;
  
  fusion::vector3<
    environment<compiled_function>::const_iterator, bool, scope::size_type
  > func = (*variables)[sym];

  if (at_c<1>(func)) {
    // TODO: 
    utree use(range, spirit::shallow);

    boost::shared_ptr<actor_list> flist(new actor_list);
    
    BOOST_FOREACH(utree const& e, use) {
      // TODO: using visit_ref will replace the need for this const_cast
      /* IMPLEMENT: uncomment when argument_visitor is implemented
      argument_visitor arg_visitor(const_cast<evaluator*>(&evaluator_visitor)); 
      flist->push_back(utree::visit(e, arg_visitor)); */
      flist->push_back(utree::visit(e, *this));
    }

    // handle placeholder-as-procedure
    if (at_c<0>(func)->second.target<placeholder>())
      return fusion::vector2<result_type, bool>(trampoline_function
        (flist, *at_c<0>(func)->second.target<placeholder>()), true);

    return fusion::vector2<result_type, bool>
      ((at_c<0>(func)->second)(flist), true);
  }

  return fusion::vector2<result_type, bool>(result_type(), false);
} // }}}

template<class Iterator>
evaluator_visitor::result_type
evaluator_visitor::operator() (iterator_range<Iterator> const& range) const {
  // {{{ nary variable/macro implementation
  using boost::fusion::at_c;

  typename iterator_range<Iterator>::const_iterator it = range.begin(),
                                                    end = range.end();

  // TODO: replace with exception
  BOOST_ASSERT(it != end);

  if (is_utree_container(*it))
    return make_anonymous_call(range);

  std::string sym = get_symbol(*it);
  ++it;
  
  // nullary call
  if (it == end) {
    // TODO: replace with exceptions
    BOOST_ASSERT(sym != "variable");
    BOOST_ASSERT(sym != "macro");
    BOOST_ASSERT(sym != "quote");
    BOOST_ASSERT(sym != "lambda");
  }

  if (sym == "variable") {
    utree body(iterator_range<Iterator>(it, end), spirit::shallow);
    return define_variable(body);
  }

  if (sym == "macro") {
    utree body(iterator_range<Iterator>(it, end), spirit::shallow);
    return define_macro(body);
  }

  if (sym == "quote")
    return quote(*it);

  if (sym == "lambda") {
    utree body(iterator_range<Iterator>(it, end), spirit::shallow);
    return make_lambda(body);
  }

  fusion::vector2<result_type, bool> r
    = expand_macro(sym, iterator_range<Iterator>(it, end));

  // expand_macro evalutes the expansion, so if a macro was found, we're done. 
  if (at_c<1>(r))
    return at_c<0>(r);

  r = make_named_call(sym, iterator_range<Iterator>(it, end));

  // if we found a named procedure, return it. 
  if (at_c<1>(r))
    return at_c<0>(r); 

  // TODO: replace with exception (identifier not found error)
  BOOST_ASSERT(false);
  return result_type();
} // }}}

// forwarders
evaluator::result_type evaluator::operator() (utree const& ut) {
  evaluator_visitor eval(&variables, &macros);
  return utree::visit(ut, eval);
}

evaluator_visitor::result_type
evaluator_visitor::make_lambda (utree const& ut) const {
  bool fixed_arity = (get_symbol(ut.front().back()) != "...");

  evaluator local(variables, macros);

  utree::const_iterator it = ut.front().begin();
  
  for (utree::size_type i = 0; i < ut.front().size(); ++i, ++it) {
    if (!fixed_arity && ((ut.front().size() - 1) == i)) {
      local.variables.define(get_symbol(*it),
        placeholder(boost::shared_ptr<function>
          (new function(phxpr::varg(i, local.variables.level()))),
           local.variables.level()));
      break;
    }

    local.variables.define(get_symbol(*it),
      placeholder(boost::shared_ptr<function>
        (new function(phxpr::arg(i, local.variables.level()))),
         local.variables.level()));
  }

  boost::shared_ptr<actor_list> flist(new actor_list);

  it = ut.begin(); ++it;

  for (; it != ut.end(); ++it) {
    if (!is_define(*it))
      flist->push_back(local(*it));
    else
      local(*it);
  }

  // REVIEW: pass fixed_arity to the function instance that's returned?
  if (flist->size() > 1)
    return function
      (begin(flist).f, ut.front().size(), fixed_arity, local.variables.level());
  else
    return function(flist->front().f,
      ut.front().size(), fixed_arity, local.variables.level());
}

} // phxpr
} // prana
} // spirit
} // boost

