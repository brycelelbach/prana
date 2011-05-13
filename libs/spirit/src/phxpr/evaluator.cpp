////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <typeinfo>
#include <boost/assert.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <boost/spirit/home/prana/support/utree_function_cast.hpp>
#include <boost/spirit/home/prana/support/utree_scope_io.hpp>
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
  
  result_type operator() (utree::invalid_type) const {
    // TODO: make this an exception
    BOOST_ASSERT(false);
    return result_type();
  }

  template<class T>
  result_type operator() (T const& val) const {
    return result_type(utree(val)); 
  }

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

  result_type operator() (any_ptr const& p) const {
    // TODO: make this an exception
    BOOST_ASSERT(false);
    return result_type();
  }

  result_type operator() (function_base const& pf) const {
    // TODO: make this an exception
    BOOST_ASSERT(false);
    return result_type();
  }

  // forwarder
  result_type operator() (utree const& ut) const {
    return utree::visit(ut, *this);
  }

  result_type define_variable (utree const& ut) const {
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
  }
  
  result_type define_macro (utree const& ut) const {
    utree::const_iterator it = ut.begin(), end = ut.end();
    
    // TODO: replace with exception
    BOOST_ASSERT(it != end);

    std::string name = get_symbol(*it);
    ++it;

    // TODO: replace with exception
    BOOST_ASSERT(it != end);

    // TODO: replace with exception (multiple definitions)
    BOOST_ASSERT(!macros->locally_defined(name));

    // IMPLEMENT

    return result_type(); 
  }
  
  result_type make_lambda (utree const& ut) const;
}; 

evaluator::~evaluator (void) {
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
}

struct trampoline_function: actor<trampoline_function> {
  boost::shared_ptr<placeholder> ph;
  boost::shared_ptr<actor_list> elements;

  typedef utree result_type;

  trampoline_function (placeholder const& ph_,
                       boost::shared_ptr<actor_list> const& elements_):
    ph(new placeholder(ph_.body, ph_.level)), elements(elements_)
  {
    BOOST_ASSERT(ph);
    BOOST_ASSERT(elements);
  }
  
  utree eval (scope const& env) const {
//    std::cout << "====================================================" << std::endl
//              << env << std::endl
//              << "====================================================" << std::endl;

    scope const* outer = &env; // Get the parent scope.

    while ((ph->level - 1) != outer->level())
      outer = outer->outer();

//    std::cout << "outer level: " << outer->level() << std::endl;

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
};
  
template<class Iterator>
evaluator_visitor::result_type
evaluator_visitor::operator() (iterator_range<Iterator> const& range) const {
  // {{{ nary variable/macro implementation
  using boost::fusion::at_c;

  typename iterator_range<Iterator>::const_iterator it = range.begin(),
                                                    end = range.end();

  // TODO: replace with exception
  BOOST_ASSERT(it != end);

  // TODO: consider moving this to a separate method
  // {{{ anonymous lambda or indirect procedure call 
  if (is_utree_container(*it)) {
    boost::shared_ptr<function> anon(new function);
    *anon = utree::visit(*it, *this);
    
    ++it;

    utree use(iterator_range<Iterator>(it, range.end()), spirit::shallow);

    boost::shared_ptr<actor_list> flist(new actor_list);

    BOOST_FOREACH(utree const& e, use) {
      flist->push_back(utree::visit(e, *this));
    }

    return lambda_function(flist, anon, variables->level()); 
  } // }}}

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
    utree body(iterator_range<Iterator>(it, range.end()), spirit::shallow);
    return define_variable(body);
  }

  if (sym == "macro") {
    utree body(iterator_range<Iterator>(it, range.end()), spirit::shallow);
    return define_macro(body);
  }

  if (sym == "quote")
    return quote(*it);

  if (sym == "lambda") {
    utree body(iterator_range<Iterator>(it, range.end()), spirit::shallow);
    return make_lambda(body);
  }

  { // {{{ macro expansion
    fusion::vector3<
      environment<macro>::const_iterator, bool, scope::size_type
    > macro = (*macros)[sym];

    if (at_c<1>(macro)) {
      utree use(iterator_range<Iterator>(it, range.end()), spirit::shallow);

      boost::shared_ptr<matcher> match = at_c<0>(macro)->second.match(use);

      if (match) {
        boost::shared_ptr<utree> expansion = match->expand();
        // TODO: make this an exception
        BOOST_ASSERT(expansion);
        return utree::visit(*expansion, *this);
      }
    }
  } // }}}

  // {{{ function creation
  fusion::vector3<
    environment<compiled_function>::const_iterator, bool, scope::size_type
  > func = (*variables)[sym];

  if (at_c<1>(func)) {
    utree use(iterator_range<Iterator>(it, range.end()), spirit::shallow);

    boost::shared_ptr<actor_list> flist(new actor_list);
    
    BOOST_FOREACH(utree const& e, use) {
      flist->push_back(utree::visit(e, *this));
    }

    // handle placeholder-as-procedure
    if (at_c<0>(func)->second.target<placeholder>()) {
      return trampoline_function
        (*at_c<0>(func)->second.target<placeholder>(), flist);
    }

    return (at_c<0>(func)->second)(flist);
  } // }}}

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
    return function
      (flist->front().f, ut.front().size(), fixed_arity, local.variables.level());
}

} // phxpr
} // prana
} // spirit
} // boost

