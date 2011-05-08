////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_B9D84807_053E_486C_A47D_AE22F28D5A9B)
#define BSP_B9D84807_053E_486C_A47D_AE22F28D5A9B

#include <boost/assert.hpp>
#include <boost/function.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <boost/spirit/home/prana/phxpr/core/actor.hpp>
#include <boost/spirit/home/prana/phxpr/core/arguments.hpp>
#include <boost/spirit/home/prana/phxpr/core/variable_arguments.hpp>
#include <boost/spirit/home/prana/phxpr/core/function.hpp>
#include <boost/spirit/home/prana/phxpr/core/quote.hpp>
#include <boost/spirit/home/prana/phxpr/core/procedure.hpp>
#include <boost/spirit/home/prana/phxpr/intrinsics/begin.hpp>
#include <boost/spirit/home/prana/phxpr/macro.hpp>
#include <boost/spirit/home/prana/phxpr/environment.hpp>

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

typedef boost::function<phxpr::function(actor_list const&)> compiled_function;

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
    return quote(utree(val));
  }

  result_type operator() (utf8_symbol_range_type const& str) const {
    // {{{ nullary variable/macro implementation
    using boost::fusion::at_c;

    std::string sym(str.begin(), str.end());

    // TODO: make this an exception
    BOOST_ASSERT(sym != "variable");
    BOOST_ASSERT(sym != "macro");
    BOOST_ASSERT(sym != "quote");
    BOOST_ASSERT(sym != "lambda");

    { // {{{ macro expansion
      fusion::vector2<environment<macro>::const_iterator, bool> macro
        = (*macros)[sym];

      if (at_c<1>(macro)) {
        boost::shared_ptr<matcher> match
          = at_c<0>(macro)->second.match(utree::list_type());

        if (match) {
          boost::shared_ptr<utree> expansion = match->expand();
          // TODO: make this an exception
          BOOST_ASSERT(expansion);
          return utree::visit(*expansion, *this);
        }
      }
    } // }}}

    // {{{ function invocation
    fusion::vector2<environment<compiled_function>::const_iterator, bool> func
      = (*variables)[sym];

    if (at_c<1>(func)) {
      actor_list flist;
      
      return (at_c<0>(func)->second)(flist);
    } // }}}
    
    // TODO: replace with exception (identifier not found error)
    BOOST_ASSERT(false);
    return result_type();
  } // }}}

  template<class Iterator>
  result_type operator() (iterator_range<Iterator> const& range) const {
    // {{{ nary variable/macro implementation
    using boost::fusion::at_c;

    typename iterator_range<Iterator>::const_iterator it = range.begin(),
                                                      end = range.end();

    // TODO: replace with exception
    BOOST_ASSERT(it != end);

    std::string sym = get_symbol(*it);
    ++it;
    
    // TODO: replace with exception
    BOOST_ASSERT(it != end);

    if (sym == "variable") {
      utree body(iterator_range<Iterator>(it, range.end()), spirit::shallow);
      return define_variable(body);
    }

    if (sym == "macro") {
      utree body(iterator_range<Iterator>(it, range.end()), spirit::shallow);
      return define_macro(body);
    }

    if (sym == "quote") {
      utree body(iterator_range<Iterator>(it, range.end()), spirit::shallow);
      return make_quote(body);
    }

    if (sym == "lambda") {
      utree body(iterator_range<Iterator>(it, range.end()), spirit::shallow);
      return make_lambda(body);
    }

    { // {{{ macro expansion
      fusion::vector2<environment<macro>::const_iterator, bool> macro
        = (*macros)[sym];

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

    // {{{ function invocation
    fusion::vector2<environment<compiled_function>::const_iterator, bool> func
      = (*variables)[sym];

    if (at_c<1>(func)) {
      utree use(iterator_range<Iterator>(it, range.end()), spirit::shallow);

      actor_list flist;

      BOOST_FOREACH(utree const& e, use) {
        flist.push_back(utree::visit(e, *this));
      }


      return (at_c<0>(func)->second)(flist);
    } // }}}

 
    // TODO: replace with exception (identifier not found error)
    BOOST_ASSERT(false);
    return result_type();
  } // }}}

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
    func->fixed = r.fixed;
    
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
  
  result_type make_quote (utree const& ut) const {
    // IMPLEMENT
    return result_type();  
  }
  
  result_type make_lambda (utree const& ut) const;
}; 

struct evaluator {
  environment<compiled_function> variables;
  environment<macro> macros;

  typedef phxpr::function result_type;

  evaluator (void) { }

  evaluator (environment<compiled_function>* variables_parent,
             environment<macro>* macros_parent):
    variables(variables_parent), macros(macros_parent) { }

  ~evaluator (void) {
    if (variables.level() == 0) {
      // Clean up recursive calls. Without help, shared_ptr<> won't release
      // these properly.
      BOOST_FOREACH(environment<compiled_function>::value_type const& e,
                    variables.definitions)
      {
        if (e.second.target<procedure>()) {
          e.second.target<procedure>()->body->f.clear();
        }
      } 
    }
  }

  // forwarders
  result_type operator() (utree const& ut) {
    evaluator_visitor eval(&variables, &macros);
    return utree::visit(ut, eval);
  }
 
  template<class T> 
  result_type operator() (T const& val) {
    evaluator_visitor eval(&variables, &macros);
    return eval(val);
  }
}; 
  
evaluator_visitor::result_type
evaluator_visitor::make_lambda (utree const& ut) const {
  bool fixed_arity = (get_symbol(ut.front().back()) != "...");

  evaluator local(variables, macros);

  utree::const_iterator it = ut.front().begin();
  
  for (utree::size_type i = 0; i < ut.front().size(); ++i, ++it) {
    if (!fixed_arity && ((ut.front().size() - 1) == i)) {
      local.variables.define(get_symbol(*it),
        procedure(boost::shared_ptr<function>
          (new function(phxpr::varg(i, local.variables.level()).f, false)),
           local.variables.level()));
      break;
    }

    local.variables.define(get_symbol(*it),
      procedure(boost::shared_ptr<function>
        (new function(phxpr::arg(i, local.variables.level()).f)),
         local.variables.level()));
  }

  boost::shared_ptr<actor_list> flist(new actor_list(ut.size() - 1));

  it = ut.begin(); ++it;

  for (; it != ut.end(); ++it) {
    if (!is_define(*it))
      flist->push_back(local(*it));
    else
      local(*it);
  }

  if (flist->size() > 1)
    return begin(flist);
  else
    return flist->front();   
}

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_B9D84807_053E_486C_A47D_AE22F28D5A9B

